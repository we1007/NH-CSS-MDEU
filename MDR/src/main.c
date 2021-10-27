#include "Allinclude.h"

int fdSerial; //串口句柄
extern int Rx_Length;
extern int returndata;

uchar8 FingerUpDataCmd[] = {0x55,0x00,0x05,0xDA,0XAA};  //指纹跟新模式
uchar8 LogFileCmdS[] = {0x55,0x00,0x05,0xDD,0XAA};  //
uchar8 LogFileCmdE[] = {0x55,0x00,0x05,0xDF,0XAA};  //

int UdiskState = 0;  //U盘当前状态 
int PrintSure;
int PrintState; //打印状态判断

char *TXTH = "/tmp/run/mountd/mmcblk0p1/LogFile/";  //TF卡文件路径
char *TXTT = ".txt";
uchar8 NowTime[3]={0};              //now  time现在MCU的时间
uchar8 CompareTime[3]={0x55,0x55,0x55}; //old  time上一次MCU的时间
uchar8 OldTime[12];
short  SerialNumber = 0; //当前操作记录号
uchar8 *name= NULL; //TF卡中时间命名的文件
uchar8 Uartbuf[14];

/*开启串口设备*/
int OpenttyDevice(void)
{
    /*2.打开串口设备*/
    fdSerial = open(DEV_NAME, O_RDWR | O_NOCTTY | O_NDELAY);
    if(fdSerial < 0)
    {
        perror(DEV_NAME);
        return -1;
    }

    // 设置串口阻塞， 0：阻塞， FNDELAY：非阻塞
    if (fcntl(fdSerial, F_SETFL, 0) < 0)    //阻塞，即使前面在open串口设备时设置的是非阻塞的
        printf("fcntl failed!\n");
    else
        printf("fcntl=%d\n", fcntl(fdSerial, F_SETFL, 0));

    if (isatty(fdSerial) == 0)
    {
        close(fdSerial);
        return -1;
    }
    else
        printf("is a tty success!\n");

    // 设置串口参数
    if (setOpt(fdSerial, 57600, 8, 'N', 1)== -1)    //设置8位数据位、1位停止位、无校验
    {
        fprintf(stderr, "Set opt Error\n");
        close(fdSerial);
        exit(1);
    }
    tcflush(fdSerial, TCIOFLUSH);    //清掉串口缓存
    fcntl(fdSerial, F_SETFL, 0);    //串口阻塞
}

/*开启守护线程*/
void DaemonThreadFunc(void)
{
    int i = 0;
    pid_t pc;
    int fd;   

    /*1.守护进程开启*/
    pc = fork(); //第一步 创建子进程，父进程退出
    if(pc<0)
    {        
        exit(1);
    }
    else if(pc>0)
        exit(0);

    setsid(); //第二步 子进程创建新会话 进程组，会话期 
    chdir("/"); //第三步 改变当前目录位根目录
    umask(0); //第四步 重设文件权限掩码
    for(i=0;i<MAXFILE;i++) //第五步关闭文件描述符
    {
        close(i);
    }      

    if((fd=open(USR,O_CREAT|O_WRONLY|O_APPEND,0600))<0)
    {
        perror("open");
        exit(1);
    } 
}



int main(int argc,char *argv[])
{
    uchar8 *MTK_Rx = NULL;
    uchar8 *TCPcmd = NULL;
    int i = 0;
    uchar8 server[20];

    FILE *Fp = NULL;
    FILE *Lp = NULL;

    /*1.开启守护进程*/
    DaemonThreadFunc();   
    /*2.开启串口设备*/
    OpenttyDevice();
    /*3.UDP初始化*/
    Udp_init();
    /*4.TCP初始化*/
    TCP_init();

    while(1)
    {      
        /*1.判断U盘是否存在*/
        Fp = fopen(FingerAddr,"r");
        if(Fp)
        {   
            if(UdiskState == 0)
            {
                UART_Send(fdSerial,FingerUpDataCmd,5);     
            }                                         
            fclose(Fp);
        }

        Lp = fopen(LogAddr,"r");
        if(Lp)
        {   
            if(UdiskState == 0)
            {
                UART_Send(fdSerial,LogFileCmdS,5);     
            }                                         
            fclose(Lp);
        }

        /*2.串口数据接收处理*/    
        MTK_Rx = UART_Recv(fdSerial,40,1000);
        if(MTK_Rx)
        {
            Receive_MCU_Data_analysis(MTK_Rx,Rx_Length);                     
        }  
        /*3.TCP*/
        Mtktcp();
        usleep(100000);     //线程睡眠100ms 
    }
    return 0;
}

/**@fn 
 * @brief 串口数据解析 MCU->MTK串口数据解析
 * 1.检测时间，并创建当日的文件
 * 2.写入操作log到当日文件中
 * 3.指令功能
 */
int Receive_MCU_Data_analysis(uchar8 *rcv_buf,int data_len)
{   
    pid_t pid;     //定义管道描述符
    int Mloop;
    int length = 0;
    length = 0;

    for(length = 0;;) //寻找数据包头部，得到数据包头部数组下标 
    {
        if(rcv_buf[length] == 0x55 && rcv_buf[length+13] == 0xAA)    
            break;
        else if (length = data_len-1)   
        {
            length = 0;
            return 0; 
        }             
        else 
            length++;
    }

    for(Mloop = 0;Mloop<14;Mloop++)
    {
        Uartbuf[Mloop] = rcv_buf[length+Mloop];
        //printf("Uartbuf = %02x\n",Uartbuf[Mloop]);
    }

    /*1.写入日记*/
    writeFile(Uartbuf);
    /*2.指令执行*/
    switch(Uartbuf[9])
    {
        case 0x01:  //开机，开网络
        {
            Udp_App(Uartbuf[8]);
            break;
        }           
        case 0x02: //关机，关网络
        {
            system("uci set firewall.@rule[9].src_ip='192.255.255.255'");
            system("/etc/init.d/firewall restart");           
            break;
        }
        case 0xDB:  //发送更新指纹的指纹数据包
        {
            UdiskState = 1;
            pid = vfork(); //第一步 创建子进程，父进程退出
            if(pid == 0)              //在子进程是处理
            {            
                SendFingerData();
                UdiskState = 0;
                _exit(0);                
            }          
            break;
        }
        case 0xDE:
        {
            UdiskState = 1;
            usleep(50000); 
            system("scp -r /tmp/run/mountd/mmcblk0p1/LogFile/ /tmp/run/mountd/sda1");
            UART_Send(fdSerial,LogFileCmdE,5);
            system("umount /tmp/run/mountd/sda1");
        }
        default:
            break;
    }
}

void writeFile(uchar8 *buf)
{
    int cmpValue,x;
    uchar8 TXTBuffer[20]; //写txt 的数据暂存Buffer
    uchar8 WriteTxT[40]; //TF卡中时间命名的文件
    uchar8 str[6]; //字符转换
    FILE *TFp = NULL;

    NowTime[0] = buf[2]; //年
    NowTime[1] = buf[3]; //月
    NowTime[2] = buf[4]; //日

    /*比较年月日，判读是否为同一天*/
    cmpValue = strcmp(NowTime,CompareTime);
    /*1.更新文件夹名称*/
    if(cmpValue != 0)
    {
        CompareTime[0] = NowTime[0];//年
        CompareTime[1] = NowTime[1];//月
        CompareTime[2] = NowTime[2];//日
        sprintf(str,"%02d%02d%02d",NowTime[0],NowTime[1],NowTime[2]);
        name = (uchar8 *) malloc(strlen(TXTH) + strlen(str)+strlen(TXTT)+3);
        strcpy(name, TXTH);
        strcat(name, str);
        strcat(name, TXTT);
        SerialNumber = 0;
    }

    TFp = fopen(name,"a");
    if(TFp)
    {            
        SerialNumber++;
        TXTBuffer[0] = (SerialNumber>>8)&0xff;
        TXTBuffer[1] = SerialNumber&0xff;
        for(x = 0;x<10;x++)
        {
            TXTBuffer[2+x] = buf[x+2];                
        }
        TXTBuffer[12] ='\r';
        TXTBuffer[13] ='\n';

        sprintf(WriteTxT,"%02x%c%02x%c%02x%c%02x%c%02x%c%02x%c%02x%c%02x%c%02x%c%02x%c%02x%c%02x%c%c",
                            TXTBuffer[0],' ',TXTBuffer[1],' ',TXTBuffer[2],' ',
                            TXTBuffer[3],' ',TXTBuffer[4],' ',TXTBuffer[5],' ',
                            TXTBuffer[6],' ',TXTBuffer[7],' ',TXTBuffer[8],' ',
                            TXTBuffer[9],' ',TXTBuffer[10],' ',TXTBuffer[11],TXTBuffer[12],TXTBuffer[13]);
        fwrite(WriteTxT,37,1,TFp);  
        fclose(TFp);
    }
}
