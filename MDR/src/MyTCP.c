#include "Allinclude.h"

#define SERV_PORT 54543   //服务器接听端口号
#define BACKLOG   20      //请求队列中允许请求数

#define  TCPUSR  "/usr/tcplog" //守护线程输出文件

uchar8 TcpPrintAnswer[] = {0x55,0x00,0x05,0x05,0XAA};  //指纹跟新模式

int ret;
int TCPsockfd;       //定义sock描述符
int clientfd; //定义数据传输sock描述符
struct sockaddr_in host_addr;   //本机IP地址和端口信息
struct sockaddr_in client_addr; //客户端IP地址和端口信息
int Tcplength = sizeof client_addr;
int recvlen = 0;
char ip[100]; 

/*TCP_init */
int TCP_init(void)
{
    int on = 1;
    /*2.创建套接字*/
    TCPsockfd = socket(AF_INET, SOCK_STREAM, 0); //TCP/IP协议，数据流套接字
    if(TCPsockfd == -1)  //判断socket函数的返回值
    {
        printf("set up socket fail.\n");
        return 0;
    }
    /*1.绑定套接字*/
    bzero(&host_addr, sizeof host_addr);
    host_addr.sin_family = AF_INET;             //TCP/IP协议
    host_addr.sin_port = htons(SERV_PORT);     //设定端口号
    host_addr.sin_addr.s_addr = INADDR_ANY;       //本地IP地址
    ret = bind(TCPsockfd, (struct sockaddr *)&host_addr, sizeof host_addr); //绑定套接字
    if(ret == -1) //判断bind函数的返回值
    {
        printf("invoking bind fail.\n");
        return 1;
    }
    /*3.监听网络端口*/
    ret = listen(TCPsockfd, BACKLOG);
    if(ret == -1)  //判断listen函数的返回值
    {
        printf("invoking listen fail.\n");
        return 1;
    } 
}


int Mtktcp(void)
{   
    uchar8 server[20];
    int clen = 0;

    /*3.TCP服务器*/        
    clientfd = accept(TCPsockfd, (struct sockaddr *)&client_addr, &Tcplength);  //接收接连请求
    if(clientfd != -1)
    {    
        /*firewall change*/              
        strcpy(ip,"uci set firewall.@rule[9].src_ip=");
        strcat(ip, inet_ntoa(client_addr.sin_addr));
        system(ip);
        system("/etc/init.d/firewall restart"); 
        
        /*1.数据接收*/
        recvlen = recv(clientfd, server, 20,0);        
        if(recvlen > 0)
        {
            for(clen = 0;clen < 16;) //寻找数据包头部，得到数据包头部数组下标 
            {
                if(server[clen] == 0x55)   
                    break;               
                else 
                    clen++;
            }
            if(server[clen+3] == 0x01)
            {
                UART_Send(fdSerial,TcpPrintAnswer,5);
            }
            else 
                printf("Tcp don't receive data.\n");                       
        }   
        /*父进程关闭套接字，准备下一个客户端连接*/    
        close(clientfd);
    }
}
