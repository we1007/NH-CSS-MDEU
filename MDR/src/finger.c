#include "Allinclude.h"

uchar8 FingerEndCmd[] = {0x55,0x00,0x05,0xDC,0XAA};     //指纹发送完成

/**@fn 
 * @brief 发送指纹数据包
 * 
 * 
 */
int SendFingerData(void)
{
    int t = 0;   
    int Fpcount = 0;
    int size;
    struct stat statbuf; 
    uchar8 ch;
    uchar8 Rbuf[1024];
    FILE *Fp;

    usleep(50000);
    //状态更改为正在读取文件 
    /*计算文件大小*/
    stat(FingerAddr,&statbuf);
    size = statbuf.st_size;
    /*读取文件*/
    Fp = fopen(FingerAddr,"rb");
    Rbuf[0] = 0x55; //包头
    Rbuf[1] = 0x03; //长度H
    Rbuf[2] = 0xF8; //长度L
    Rbuf[3] = 0xDA; //command
    
    fseek(Fp,0,SEEK_SET);

    for(t = 0;t<size+1;t++)
    {
        ch = getc(Fp);
        Rbuf[4+Fpcount] = ch;
        if(Fpcount == 1010)
        {           
            Rbuf[1015] = 0xAA; //尾
            UART_Send(fdSerial, Rbuf, 1016);
            Fpcount = 0;
            usleep(2000000); //2000ms
        }
        else 
            Fpcount++;
    }    
    fclose(Fp);

    //状态更改为文件读取完成
    system("umount /tmp/run/mountd/sda1");
    UART_Send(fdSerial,FingerEndCmd,5);  
    usleep(150000);  //150ms
    UART_Send(fdSerial,FingerEndCmd,5);  

    return 1;
}
