#ifndef __ALL_H
#define __ALL_H

#include <stdio.h>
#include <stdlib.h>         //标准库头文件，定义了五种类型、一些宏和通用工具函数
#include <string.h>         //字符串操作
#include <unistd.h>         //定义 read write close lseek 等Unix标准函数
#include <sys/types.h>      //定义数据类型，如 ssiz e_t off_t 等
#include <sys/stat.h>       //文件状态
#include <fcntl.h>          //文件控制定义
#include <termios.h>        //终端I/O
#include <errno.h>          //与全局变量 errno 相关的定义
#include <getopt.h>         //处理命令行参数
#include <sys/select.h>     //select函数
#include<sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include<netdb.h>

typedef unsigned char uchar8;

/*串口*/
#define MAXFILE 10
#define DEV_NAME "/dev/ttyS0"  //< 串口设备 >
#define  USR  "/usr/log" //守护线程输出文件
#define  FingerAddr  "/tmp/run/mountd/sda1/PrinterBuffer/finger" //U盘输入文件
#define  LogAddr "/tmp/run/mountd/sda1/LogFile"


extern int fdSerial; //串口句柄
extern int UdiskState;  //U盘当前状态 

int Receive_MCU_Data_analysis(uchar8 *rcv_buf,int data_len);
void writeFile(uchar8 *buf);
int SendFingerData(void);
void DaemonThreadFunc(void);
int OpenttyDevice(void);
int setOpt(int fd, int nSpeed, int nBits, int nParity, int nStop);
uchar8 *UART_Recv(int fd, int data_len, int timeout);
int UART_Send(int fd, uchar8 *send_buf, int data_len);
void SendDataToMcu(void);
uchar8 sum8(const uchar8 *dat, uchar8 len);
void Udp_init(void);
int Udp_App(uchar8 Uid);
int TCP_init(void);
int Mtktcp(void);

#endif