#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f10x.h"
#include "stm32f10x_flash.h"

#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
#define FLASH_PAGE_SIZE ((uint16_t)0x800)

#else
#define FLASH_PAGE_SIZE ((uint16_t)0x400)
#endif

#define START_ADDR_GROUP_TAB     ((uint32_t)0x0800AC00)		//从43k地址开始 15k红外码组号数据
#define START_ADDR_TEAR  				 ((uint32_t)0x0800E800)  	 //58k地址开始存放其它数据

/******************************************************************************/
#define FIRSTPOWER  	START_ADDR_TEAR+4 //第一次上电标志
#define STABLE_POWER_ADDR		FIRSTPOWER+4  //稳定功率
#define SCAN_POWER_ADDR		STABLE_POWER_ADDR+4  //稳定功率ScanPowerValue
#define PRINT_POWER_ADDR		SCAN_POWER_ADDR+4  //稳定功率PrintPowerValue
#define COPY_POWER_ADDR		PRINT_POWER_ADDR+4  //稳定功率CopyPowerValue
#define DS3231FIRSTPOWER    COPY_POWER_ADDR+4
#define PRINTFCOUNT  DS3231FIRSTPOWER+4

/******************************************************************************/ 

uint8_t Flash_Read(uint32_t iAddress, uint8_t *buf, int32_t iNbrToRead);
uint8_t Flash_Write(uint32_t iAddress, const uint8_t *buf, uint32_t iNbrToWrite);
uint8_t Flash_Write_super(uint32_t iAddress, const uint8_t *buf, uint32_t iNbrToWrite);


//u8 Infr_Dat_WirteFlash(void);
#endif
