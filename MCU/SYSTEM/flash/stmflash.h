#ifndef __STMFLASH_H__
#define __STMFLASH_H__
//#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//STM32 FLASH 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stm32f10x.h"

//用户根据自己的需要设置
#define STM32_FLASH_SIZE 64 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 0x0800F400 	//STM32 FLASH的起始地址61K
#define SMT32_FLASH_START STM32_FLASH_BASE
//FLASH解锁键值
 
/******************************************************************************/
#define SHORT_ADDR_LEN     1															//Zig数据 - 短地址长度
#define ALIAS_NAME_LEN  	16															//Zig数据 - 别名长度
#define ALL_ZIG_DAT_LEN	(SHORT_ADDR_LEN+ALIAS_NAME_LEN)		//所有ZigBee数据长度

#define ONE_INFR_PROT_DAT_LEN	5														//单个红外端口数据长度
#define INFR_PORT_NUM					10													//红外端口数量
#define ALL_INFR_PORT_DAT_LEN (INFR_PORT_NUM*ONE_INFR_PROT_DAT_LEN)	//所有红外遥控数据长度
 
#define INFR_LEAR_DAT_SAV_POS_LEN		1		//红外学习数据存储当前位置数据长度（因为数据量太大,FLASH有限，只能轮流存储）
#define INFR_LEAR_DAT_LEN						(208/2 + 5)	//红外学习数据数据长度（保存标志(2B)+设备号(2B)+码组号(2B)+键值(2B)+LRC数据(208B)
#define INFR_LEAR_DAT_GROUP_NUM			8	//红外学习数据组数，一组为一个按键的学习数据
#define ALL_INFR_LEAR_DAT_LEN			(INFR_LEAR_DAT_LEN*INFR_LEAR_DAT_GROUP_NUM)//所有红外学习数据长度

#define HEAD_POS 0    																	//数据存储相对头地址位置
#define SHORT_ADDR_POS HEAD_POS													//短地址起始地址位置
#define ALIAS_NAME_POS (SHORT_ADDR_POS+SHORT_ADDR_LEN)	//别名起始地址位置
#define INFR_DAT_POS (ALIAS_NAME_POS+ALIAS_NAME_LEN)    //红外遥控数据起始地址位置

#define INFR_LEAR_DAT_SAV_NUM_POS		(INFR_DAT_POS+ALL_INFR_PORT_DAT_LEN)//红外学习当前存储位置起始地址位置
#define INFR_LEAR_DAT_POS	(INFR_LEAR_DAT_SAV_NUM_POS+INFR_LEAR_DAT_SAV_POS_LEN)	//红外学习数据起始地址为位置

#define ALL_DAT_LEN	(ALL_ZIG_DAT_LEN+ALL_INFR_PORT_DAT_LEN+INFR_LEAR_DAT_SAV_POS_LEN)//所有数据长度
/* Variables' number */
#define NumbOfVar               ((uint16_t)1) 
/******************************************************************************/ 
 
#define  HEAD_ADDR  				 SMT32_FLASH_START
#define  ZIG_SHORT_ADDR 				 HEAD_ADDR
#define  ZIG_ALIAS_NAME_ADDR		(ZIG_SHORT_ADDR+SHORT_ADDR_LEN*2) 
#define  IRD_ADDR   				(ZIG_ALIAS_NAME_ADDR+ALIAS_NAME_LEN*2)
#define  ILD_SAV_POS_ADDR   (IRD_ADDR+ALL_INFR_PORT_DAT_LEN*2)
#define  ILD_ADDR  					(2+ILD_SAV_POS_ADDR+INFR_LEAR_DAT_SAV_POS_LEN*2)

u16 STMFLASH_ReadHalfWord(u32 faddr);		  //读出半字  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//指定地址开始写入指定长度的数据
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//指定地址开始读取指定长度数据
u8 STMFLASH_Write(u32 WriteAddr,const u16 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
u8 STMFLASH_Read(u32 ReadAddr, u16 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据

void read_and_write_test(void);							   
#endif

















