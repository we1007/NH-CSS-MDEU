#ifndef __POWER_ENE_H
#define __POWER_ENE_H

#include "stm32f10x.h"


extern uint16_t REG_RMS_IAC,REG_RMS_UC,REG_RMS_POWER_PAC,REG_ENERGY_AC;  //����ϵ������ѹϵ��������ϵ��,����ϵ��

#define REG_RMSLIA_ADDR             0x24         //ͨ��A������Чֵ
#define REG_RMSLU_ADDR              0X26         //��ѹ��Чֵ
#define REG_POWER_PA_ADDR           0x2C         //������Чֵ
#define REG_ENERGY_PA_ADDR			    0x28         //������Чֵ


#define INSTAN_IA_REG               0x33        //����ͨ��A˲ʱֵ 
#define INSTAN_U_REG                0x35        //��ѹͨ��˲ʱֵ
#define INSTAN_P_REG                0x3C        //�й�����˲ʱֵ ͨ������ѡ��ͨ�� A��ͨ�� B ���й�����˲ʱֵ


#define REG_RMS_IAC_ADDR		        0x70         //ͨ��A��������
#define REG_RMS_UC_ADDR				      0x72         //��ѹ����
#define REG_POWER_PAC_ADDR			    0x73         //���ʲ���
#define REG_ENERGY_AC_ADDR			    0x76         //��������
																    
#define REG_HFCONST_ADDR            0x02
#define REG_EMUCON2_ADDR            0x13


enum
{
	CONSULT_REG_RMS_IAC        = 0xcbf7,   //�����ο�ϵ��
//	CONSULT_REG_RMS_IAC        = 0x98f7,   //�����ο�ϵ��
	CONSULT_REG_RMS_UC         = 0xa847,   //��ѹ�ο�ϵ��
	CONSULT_REG_RMS_POWER_PAC  = 0xafe7,   //���ʲο�ϵ��
//	CONSULT_REG_RMS_POWER_PAC  = 0x78e7,   //���ʲο�ϵ��
	CONSULT_REG_ENERGY_AC      = 0xebd6    //�����ο�ϵ��
};

typedef struct __RMSL_DATA{
	uint8_t Rmsl_U[2];
	uint8_t Rmsl_A[2];
	uint8_t Rmsl_P[2];
	uint8_t Rmsl_E[3];
}RMSL_DATA;

extern RMSL_DATA Rmsl_Data;

typedef struct __POWER_ENE
{
    unsigned int voltage; //��ѹ
	unsigned int electric;  //����
	double amount   ; //����
	double last_amount;      //�ɵ�����ÿ0.01�ȸ���
	double power    ; //����
	unsigned int power_limit;       //�����޶�ֵ
}POWER_ENE;


extern RMSL_DATA Rmsl_Data;

extern POWER_ENE Power_Ene;

extern unsigned char R_AutoSetSec[2] ;   //�Զ���ʱ�ر���
extern unsigned char R_AutoSetMin[2];   //�Զ���ʱ�رշ�
extern unsigned int R_AutoSetTime[2] ;   //�Զ���ʱ�ر�������
extern unsigned int R_AutoDlyCnt[2];    //�Զ���ʱʱ�����ݼ�



void HLW8110_Usart_Init(void);
void Read_Rmsl_Cmd_Time(void);
void Read_VAP_Init(void);
void Write_Rmsl_Init(void);
void Write_Amount_Init(void);
void InitData(void);
void Read_Rmsl_UC(void);
void Read_Rmsl_IAC(void);
void Read_Rmsl_POWER_PAC(void);
void Ene_Count_Init(void);


#endif
