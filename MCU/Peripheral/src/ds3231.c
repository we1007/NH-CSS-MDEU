#include "ds3231.h"
#include "delay.h"


#define DS3231_WriteAddress        0xD0  
#define DS3231_ReadAddress        0xD1

Calendar calendar;

void I2C_GPIO_Config(void)  
{  
    GPIO_InitTypeDef  GPIO_InitStructure;  
          
    RCC_APB2PeriphClockCmd(SCL_RCC_CLOCK | SDA_RCC_CLOCK ,ENABLE);  
          
    //��ʼ��SCL�ܽ�  
    GPIO_InitStructure.GPIO_Pin =  SCL_PIN;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;    
    GPIO_Init(SCL_PORT, &GPIO_InitStructure);  
          
    //��ʼ��SDA�ܽ�  
    GPIO_InitStructure.GPIO_Pin =  SDA_PIN;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
    GPIO_Init(SDA_PORT, &GPIO_InitStructure);  
}

/**************************************************************************  
* ������: void I2C_delay(void)
* ����  : ������ʱ
* ����  : ��
* ���  : ��
* ˵��  : �ڲ������i�����Ż��ٶȣ���������͵�5����д��
***************************************************************************/  
static void I2C_delay(void)  
{    
    u8 i=5;  
    while(i)  
    {  
        i--;  
    }  
}


/**************************************************************************
* ������: void I2C_Start(void)
* ����  : ��ʼ�ź�
* ����  : ��
* ���  : ��
* ˵��  :  
***************************************************************************/
void I2C_Start(void)
{
    SCL_H;    I2C_delay();
    SDA_H;    I2C_delay();
    SDA_L;    I2C_delay();
    SCL_L;    I2C_delay();
}


/**************************************************************************
* ������: I2C_Stop(void)
* ����  : ��ֹ�ź�
* ����  : ��
* ���  : ��
* ˵��  :  
***************************************************************************/
void I2C_Stop(void)
{
    SDA_L;    I2C_delay();
    SCL_H;    I2C_delay();
    SDA_H;    I2C_delay();
}


/**************************************************************************
* ������: void I2C_Ack(void)
* ����  : Ӧ���ź�
* ����  : ��
* ���  : ��
* ˵��  :  
***************************************************************************/
void I2C_Ack(void)  
{    
    SCL_L;  I2C_delay();  
    SDA_L;  I2C_delay();  
    SCL_H;  I2C_delay();  
    SCL_L;  I2C_delay();  
}


/**************************************************************************
* ������: void I2C_NoAck(void)
* ����  : ��Ӧ���ź�
* ����  : ��
* ���  : ��
* ˵��  :  
***************************************************************************/
void I2C_NoAck(void)  
{    
    SCL_L;  I2C_delay();  
    SDA_H;  I2C_delay();  
    SCL_H;  I2C_delay();  
    SCL_L;  I2C_delay();  
}  


/**************************************************************************
* ������: u8 I2C_WaitAck(void)
* ����  : �ȴ�Ӧ���ź�
* ����  : ��
* ���  : TRUE : ��Ӧ��
           FALSE : ��Ӧ��
* ˵��  :  
***************************************************************************/
u8 I2C_WaitAck(void)    
{  
    u8 ucErrTime=0;
    SCL_L;  I2C_delay();  
    SDA_H;  I2C_delay();  
    SCL_H;  I2C_delay();  
    while(SDA_read)  
    {  
        ucErrTime++;
        if(ucErrTime>250)
        {
            I2C_Stop();
            return 0;
        }
    }  
    SCL_L;  
    return 1;  
}  


/**************************************************************************  
* ������: void I2C_SendByte(u8 SendByte)  
* ����  : ����һ���ֽ�
* ����  : SendByte : �ֽ�����
* ���  : ��
* ˵��  : ���ݴӸ�λ����λ
***************************************************************************/
void I2C_SendByte(u8 SendByte)  
{  
    u8 i=8;  
    while(i--)  
    {  
        SCL_L;  
        I2C_delay();
        
        if(SendByte & 0x80)  
            SDA_H;    
        else  
            SDA_L;    
        SendByte<<=1;  
        I2C_delay();
        
        SCL_H;  
        I2C_delay();  
    }  
    SCL_L;  
}


/**************************************************************************
* ������: u8 I2C_ReceiveByte(void)  
* ����  : ��ȡһ���ֽ�
* ����  : ��  
* ���  : �ֽ�����
* ˵��  : ReceiveByte : ���ݴӸ�λ����λ
***************************************************************************/
u8 I2C_ReceiveByte(void)    
{  
    u8 i=8;  
    u8 ReceiveByte=0;  
          
    SDA_H;                
    while(i--)  
    {  
        ReceiveByte<<=1;        
        SCL_L;  
        I2C_delay();  
        SCL_H;
        I2C_delay();      
        if(SDA_read)  
        {  
            ReceiveByte|=0x01;  
        }  
    }  
    SCL_L;  
    return ReceiveByte;  
}

/**********************************************************************
ʱ��ģ�鹦��
***********************************************************************/
/**
  * @brief  
  * @retval u8
  */
u8 BCD2HEX(u8 val)
{
    u8 i;
    i= val&0x0f;
    val >>= 4;
    val &= 0x0f;
    val *= 10;
    i += val;
    
    return i;
}
u16 B_BCD(u8 val)
{
    u8 i,j,k;
    i=val/10;
    j=val%10;
    k=j+(i<<4);
    return k;
}

void DS3231_WR_Byte(u8 addr,u8 bytedata)
{
    I2C_Start();
    I2C_SendByte(DS3231_WriteAddress);
    I2C_WaitAck();
    I2C_SendByte(addr);
    I2C_WaitAck();
    I2C_SendByte(bytedata);
    I2C_WaitAck();
    I2C_Stop();
}    

u8 DS3231_RD_Byte(u8 addr)
{
    u8 Dat=0;
  
    I2C_Start();
    I2C_SendByte(DS3231_WriteAddress);
    I2C_WaitAck();
    I2C_SendByte(addr);
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(DS3231_ReadAddress);
    I2C_WaitAck();
    Dat=I2C_ReceiveByte();
    I2C_Stop();
    
    return Dat;
}

/**
  * @brief  ʱ��ģ���ʼ��
  * @retval void
  */
void DS3231_Init(void)
{
    I2C_GPIO_Config();
    DS3231_WR_Byte(DS3231_CONTROL_REG,0x00);
    delay_ms(2);
    DS3231_WR_Byte(DS3231_STATUS_REG,0x00);
    delay_ms(2);
}


void Set_DS3231_Time(u8 yea,u8 mon,u8 da,u8 week,u8 hou,u8 min,u8 sec)
{
    u8 temp=0;
  
    temp=B_BCD(yea);
    DS3231_WR_Byte(DS3231_YEAR_REG,temp);
  
    temp=B_BCD(mon);
    DS3231_WR_Byte(DS3231_MONTH_REG,temp);
  
    temp=B_BCD(da);
    DS3231_WR_Byte(DS3231_MDAY_REG,temp);
	
	  temp=B_BCD(week);
    DS3231_WR_Byte(DS3231_WDAY_REG,temp);
	
    temp=B_BCD(hou);
    DS3231_WR_Byte(DS3231_HOUR_REG,temp);
  
    temp=B_BCD(min);
    DS3231_WR_Byte(DS3231_MIN_REG,temp);
  
    temp=B_BCD(sec);
    DS3231_WR_Byte(DS3231_SEC_REG,temp);
}

void Get_DS3231_Time(void)
{
	if(DS3231_RD_Byte(0x06)<=99&&DS3231_RD_Byte(0x06)>0)
	{
    calendar.year=DS3231_RD_Byte(0x06); 
    calendar.year=BCD2HEX(calendar.year);
		//calendar.year=calendar.year+2000;

    calendar.month=DS3231_RD_Byte(0x05);
    calendar.month=BCD2HEX(calendar.month);

    calendar.day=DS3231_RD_Byte(0x04);  
    calendar.day=BCD2HEX(calendar.day);
	
    calendar.week=DS3231_RD_Byte(0x03);
    calendar.week=BCD2HEX(calendar.week);
	
    calendar.hour=DS3231_RD_Byte(0x02);
    calendar.hour&=0x3f;                  
    calendar.hour=BCD2HEX(calendar.hour);

    calendar.minute=DS3231_RD_Byte(0x01);
    calendar.minute=BCD2HEX(calendar.minute);

    calendar.second=DS3231_RD_Byte(0x00);
    calendar.second=BCD2HEX(calendar.second);
	}
	else
	{
		
	}
}
