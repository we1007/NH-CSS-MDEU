#ifndef __POWER_ENE_H
#define __POWER_ENE_H

#include "stm32f10x.h"


extern uint16_t REG_RMS_IAC,REG_RMS_UC,REG_RMS_POWER_PAC,REG_ENERGY_AC;  //电流系数，电压系数，功率系数,电量系数

#define REG_RMSLIA_ADDR             0x24         //通道A电流有效值
#define REG_RMSLU_ADDR              0X26         //电压有效值
#define REG_POWER_PA_ADDR           0x2C         //功率有效值
#define REG_ENERGY_PA_ADDR			    0x28         //电量有效值


#define INSTAN_IA_REG               0x33        //电流通道A瞬时值 
#define INSTAN_U_REG                0x35        //电压通道瞬时值
#define INSTAN_P_REG                0x3C        //有功功率瞬时值 通过命令选择通道 A或通道 B 的有功功率瞬时值


#define REG_RMS_IAC_ADDR		        0x70         //通道A电流参数
#define REG_RMS_UC_ADDR				      0x72         //电压参数
#define REG_POWER_PAC_ADDR			    0x73         //功率参数
#define REG_ENERGY_AC_ADDR			    0x76         //电量参数
																    
#define REG_HFCONST_ADDR            0x02
#define REG_EMUCON2_ADDR            0x13


enum
{
	CONSULT_REG_RMS_IAC        = 0xcbf7,   //电流参考系数
//	CONSULT_REG_RMS_IAC        = 0x98f7,   //电流参考系数
	CONSULT_REG_RMS_UC         = 0xa847,   //电压参考系数
	CONSULT_REG_RMS_POWER_PAC  = 0xafe7,   //功率参考系数
//	CONSULT_REG_RMS_POWER_PAC  = 0x78e7,   //功率参考系数
	CONSULT_REG_ENERGY_AC      = 0xebd6    //电量参考系数
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
    unsigned int voltage; //电压
	unsigned int electric;  //电流
	double amount   ; //电量
	double last_amount;      //旧电量，每0.01度更新
	double power    ; //功率
	unsigned int power_limit;       //功率限定值
}POWER_ENE;


extern RMSL_DATA Rmsl_Data;

extern POWER_ENE Power_Ene;

extern unsigned char R_AutoSetSec[2] ;   //自动延时关闭秒
extern unsigned char R_AutoSetMin[2];   //自动延时关闭分
extern unsigned int R_AutoSetTime[2] ;   //自动延时关闭总秒数
extern unsigned int R_AutoDlyCnt[2];    //自动延时时间计算递减



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
