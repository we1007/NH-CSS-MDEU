#ifndef __STMFLASH_H__
#define __STMFLASH_H__
//#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//STM32 FLASH ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stm32f10x.h"

//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 64 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x0800F400 	//STM32 FLASH����ʼ��ַ61K
#define SMT32_FLASH_START STM32_FLASH_BASE
//FLASH������ֵ
 
/******************************************************************************/
#define SHORT_ADDR_LEN     1															//Zig���� - �̵�ַ����
#define ALIAS_NAME_LEN  	16															//Zig���� - ��������
#define ALL_ZIG_DAT_LEN	(SHORT_ADDR_LEN+ALIAS_NAME_LEN)		//����ZigBee���ݳ���

#define ONE_INFR_PROT_DAT_LEN	5														//��������˿����ݳ���
#define INFR_PORT_NUM					10													//����˿�����
#define ALL_INFR_PORT_DAT_LEN (INFR_PORT_NUM*ONE_INFR_PROT_DAT_LEN)	//���к���ң�����ݳ���
 
#define INFR_LEAR_DAT_SAV_POS_LEN		1		//����ѧϰ���ݴ洢��ǰλ�����ݳ��ȣ���Ϊ������̫��,FLASH���ޣ�ֻ�������洢��
#define INFR_LEAR_DAT_LEN						(208/2 + 5)	//����ѧϰ�������ݳ��ȣ������־(2B)+�豸��(2B)+�����(2B)+��ֵ(2B)+LRC����(208B)
#define INFR_LEAR_DAT_GROUP_NUM			8	//����ѧϰ����������һ��Ϊһ��������ѧϰ����
#define ALL_INFR_LEAR_DAT_LEN			(INFR_LEAR_DAT_LEN*INFR_LEAR_DAT_GROUP_NUM)//���к���ѧϰ���ݳ���

#define HEAD_POS 0    																	//���ݴ洢���ͷ��ַλ��
#define SHORT_ADDR_POS HEAD_POS													//�̵�ַ��ʼ��ַλ��
#define ALIAS_NAME_POS (SHORT_ADDR_POS+SHORT_ADDR_LEN)	//������ʼ��ַλ��
#define INFR_DAT_POS (ALIAS_NAME_POS+ALIAS_NAME_LEN)    //����ң��������ʼ��ַλ��

#define INFR_LEAR_DAT_SAV_NUM_POS		(INFR_DAT_POS+ALL_INFR_PORT_DAT_LEN)//����ѧϰ��ǰ�洢λ����ʼ��ַλ��
#define INFR_LEAR_DAT_POS	(INFR_LEAR_DAT_SAV_NUM_POS+INFR_LEAR_DAT_SAV_POS_LEN)	//����ѧϰ������ʼ��ַΪλ��

#define ALL_DAT_LEN	(ALL_ZIG_DAT_LEN+ALL_INFR_PORT_DAT_LEN+INFR_LEAR_DAT_SAV_POS_LEN)//�������ݳ���
/* Variables' number */
#define NumbOfVar               ((uint16_t)1) 
/******************************************************************************/ 
 
#define  HEAD_ADDR  				 SMT32_FLASH_START
#define  ZIG_SHORT_ADDR 				 HEAD_ADDR
#define  ZIG_ALIAS_NAME_ADDR		(ZIG_SHORT_ADDR+SHORT_ADDR_LEN*2) 
#define  IRD_ADDR   				(ZIG_ALIAS_NAME_ADDR+ALIAS_NAME_LEN*2)
#define  ILD_SAV_POS_ADDR   (IRD_ADDR+ALL_INFR_PORT_DAT_LEN*2)
#define  ILD_ADDR  					(2+ILD_SAV_POS_ADDR+INFR_LEAR_DAT_SAV_POS_LEN*2)

u16 STMFLASH_ReadHalfWord(u32 faddr);		  //��������  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//ָ����ַ��ʼ��ȡָ����������
u8 STMFLASH_Write(u32 WriteAddr,const u16 *pBuffer,u16 NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
u8 STMFLASH_Read(u32 ReadAddr, u16 *pBuffer,u16 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����

void read_and_write_test(void);							   
#endif

















