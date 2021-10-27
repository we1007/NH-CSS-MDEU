#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f10x.h"
#include "stm32f10x_flash.h"

#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
#define FLASH_PAGE_SIZE ((uint16_t)0x800)

#else
#define FLASH_PAGE_SIZE ((uint16_t)0x400)
#endif

#define START_ADDR_GROUP_TAB     ((uint32_t)0x0800AC00)		//��43k��ַ��ʼ 15k�������������
#define START_ADDR_TEAR  				 ((uint32_t)0x0800E800)  	 //58k��ַ��ʼ�����������

/******************************************************************************/
#define FIRSTPOWER  	START_ADDR_TEAR+4 //��һ���ϵ��־
#define STABLE_POWER_ADDR		FIRSTPOWER+4  //�ȶ�����
#define SCAN_POWER_ADDR		STABLE_POWER_ADDR+4  //�ȶ�����ScanPowerValue
#define PRINT_POWER_ADDR		SCAN_POWER_ADDR+4  //�ȶ�����PrintPowerValue
#define COPY_POWER_ADDR		PRINT_POWER_ADDR+4  //�ȶ�����CopyPowerValue
#define DS3231FIRSTPOWER    COPY_POWER_ADDR+4
#define PRINTFCOUNT  DS3231FIRSTPOWER+4

/******************************************************************************/ 

uint8_t Flash_Read(uint32_t iAddress, uint8_t *buf, int32_t iNbrToRead);
uint8_t Flash_Write(uint32_t iAddress, const uint8_t *buf, uint32_t iNbrToWrite);
uint8_t Flash_Write_super(uint32_t iAddress, const uint8_t *buf, uint32_t iNbrToWrite);


//u8 Infr_Dat_WirteFlash(void);
#endif
