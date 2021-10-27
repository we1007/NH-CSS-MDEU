#include "Allinclude.h"

#define MyPORT 45453
#define MAXDATASIZE 256

uchar8 TxData[] = {0x55,0x06,0x00,0x00,0X07,0xAA}; 
int so_broadcast=1;
uchar8 buf[MAXDATASIZE];
struct sockaddr_in my_addr,user_addr;
int socket_fd;
    
uchar8 sum8(const uchar8 *dat, uchar8 len)
{
    int i;
	uchar8 Num = 0;
    for(i=0; i<len; i++)
    {
        Num += dat[i];
    }
    return Num;
}

void Udp_init(void)
{
    my_addr.sin_family=AF_INET;
    my_addr.sin_port=htons(MyPORT);
    my_addr.sin_addr.s_addr=inet_addr("255.255.255.255");
    bzero(&(my_addr.sin_zero),8);
   
    if((socket_fd=(socket(AF_INET,SOCK_DGRAM,0)))==-1) 
    {
        printf("Udp_init socket_fd error.\n");
        perror("socket");
        exit(1);
    }
    setsockopt(socket_fd,SOL_SOCKET,SO_BROADCAST,&so_broadcast,sizeof(so_broadcast));
 
}

int Udp_App(uchar8 Uid)
{
    socklen_t size;
    TxData[2] = Uid;
    TxData[4] = sum8(TxData+1,3);
    sendto(socket_fd,TxData,6,0,(struct sockaddr *)&my_addr,sizeof(my_addr)); /*发送*/
    
    return 0;
}

