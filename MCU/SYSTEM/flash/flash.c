#include "flash.h"
#include "UartPrintf.h" 
#include "string.h"

uint16_t Flash_Write_Without_check(uint32_t iAddress, const uint8_t *buf, uint16_t iNumByteToWrite)
{
    uint16_t i;
    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
    i = 0;

    // FLASH_UnlockBank1();
    while((i < iNumByteToWrite) && (FLASHStatus == FLASH_COMPLETE))
    {
        FLASHStatus = FLASH_ProgramHalfWord(iAddress, *(uint16_t*)buf); //��
        i = i+2;
        iAddress = iAddress + 2;
        buf = buf + 2;
    }

    return iNumByteToWrite;
}
/**
* @brief Programs a half word at a specified Option Byte Data address.
* @note This function can be used for all STM32F10x devices.
* @param Address: specifies the address to be programmed.
* @param buf: specifies the data to be programmed.
* @param iNbrToWrite: the number to write into flash
* @retval if success return the number to write, -1 if error
* 
*/
uint8_t Flash_Write(uint32_t iAddress, const uint8_t *buf, uint32_t iNbrToWrite) 
{
    /* Unlock the Flash Bank1 Program Erase controller */
    uint32_t secpos;
    uint32_t iNumByteToWrite = iNbrToWrite;
    uint16_t secoff;
    uint16_t secremain; 
    uint16_t i = 0; 
    uint8_t tmp[FLASH_PAGE_SIZE];
	  volatile FLASH_Status FLASHStatus;
		
    FLASH_UnlockBank1();
    secpos=iAddress & (~(FLASH_PAGE_SIZE -1 )) ;//������ַ 
    secoff=iAddress & (FLASH_PAGE_SIZE -1); //�������ڵ�ƫ��
    secremain=FLASH_PAGE_SIZE-secoff; //����ʣ��ռ��С 
    FLASHStatus = FLASH_COMPLETE;

    if(iNumByteToWrite<=secremain) secremain = iNumByteToWrite;//������4096���ֽ�

    while( 1 ) 
    {
        Flash_Read(secpos, tmp, FLASH_PAGE_SIZE); //������������
        for(i=0;i<secremain;i++) 
        { //У������
            if(tmp[secoff+i]!=0XFF)break; //��Ҫ���� 
        }
        if(i<secremain) 
        { //��Ҫ����
            FLASHStatus = FLASH_ErasePage(secpos); //�����������
            if(FLASHStatus != FLASH_COMPLETE)
            return 0;
            for(i=0;i<secremain;i++) //����
            { 
                tmp[i+secoff]=buf[i]; 
            }
            Flash_Write_Without_check(secpos ,tmp ,FLASH_PAGE_SIZE);//д���������� 
        } 
        else 
        {
            Flash_Write_Without_check(iAddress,buf,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.
        }

        if(iNumByteToWrite==secremain) //д�������
        {
            break;
        }
        else 
        {
            secpos += FLASH_PAGE_SIZE;
            secoff = 0;//ƫ��λ��Ϊ0 
            buf += secremain; //ָ��ƫ��
            iAddress += secremain;//д��ַƫ�� 
            iNumByteToWrite -= secremain; //�ֽ����ݼ�
            if(iNumByteToWrite>FLASH_PAGE_SIZE) secremain=FLASH_PAGE_SIZE;//��һ����������д����
            else secremain = iNumByteToWrite; //��һ����������д����
        }
    }
    FLASH_LockBank1();
    return 1; 
}

/**
* @brief Programs a half word at a specified Option Byte Data address.
* @note This function can be used for all STM32F10x devices.
* @param Address: specifies the address to be programmed.
* @param buf: specifies the data to be programmed.
* @param iNbrToWrite: the number to read from flash
* @retval if success return the number to write, without error
* 
*/
uint8_t Flash_Read(uint32_t iAddress, uint8_t *buf, int32_t iNbrToRead) 
{
    int i = 0;
    while(i < iNbrToRead) 
    {
        *(buf + i) = *(__IO uint8_t*) iAddress++;
        i++;
    }
    return 1;
}

//����дFlash������ַ		һ�������д12���ֽ�
uint8_t Flash_Write_super(uint32_t iAddress, const uint8_t *buf, uint32_t iNbrToWrite)
{	
	u8 Flash_temp[2] = {0};
	u8 temp = 0;
	u8 temp1 = 0;
	u8 temp2[16] = {0};
	if((iAddress%2 != 0) && (iNbrToWrite%2 != 0))	//��ַΪ���������ֽ�Ϊ����
	{
		Flash_Read(iAddress-1, &Flash_temp[0], 1);  //������ַǰһ���ֽ�
		Flash_temp[1] = (*buf); 
		Flash_Write(iAddress-1,(const u8 *)&Flash_temp,2);	//ż����ַд�������ֽ�
		if(iNbrToWrite > 2)							//�ֽ�������2���ֽ� ����дʣ�µ��ֽ�
			Flash_Write(iAddress+1,buf+1,iNbrToWrite-1);
			
	}
	else if((iAddress%2 == 0) && (iNbrToWrite%2 != 0))   //��ַΪż�������ֽ�Ϊ����
	{
		Flash_Read(iAddress+iNbrToWrite, &temp, 1);  //���������ֽں�һ���ֽ�
		memcpy(temp2, buf, iNbrToWrite);  //buf���Ƴ���
		temp2[iNbrToWrite] = temp;        //��һ��ֵ��ֵ
		Flash_Write(iAddress,temp2,iNbrToWrite+1);		//��bufд��Flash
	}
	else if((iAddress%2 != 0) && (iNbrToWrite%2 == 0))    //��ַΪ���� �ֽ�Ϊż��
	{
		Flash_Read(iAddress-1, &temp1, 1);  //������ַǰһ���ֽ�
		Flash_Read(iAddress+iNbrToWrite, &temp, 1);  //���������ֽں�һ���ֽ�
		temp2[0] = temp1;
		memcpy(&temp2[1], buf, iNbrToWrite);
		temp2[iNbrToWrite+1] = temp;
		Flash_Write(iAddress-1,temp2,iNbrToWrite+2);	//��addrǰһ����ַ��ʼ����bufǰһ���ֽڵ�����д��Flash��һ��buf+2���ֽ�
	}
	else if((iAddress%2 == 0) && (iNbrToWrite%2 == 0))		//��ַΪż�� �ֽ�Ϊż��
		Flash_Write(iAddress, buf, iNbrToWrite);  //ż����ַ ֱ��д
	
	return 1;
	
}

