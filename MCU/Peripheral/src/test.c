/****************************************************************************************************
//=========================================电源接线================================================//
// OLED模块               STM32单片机
//   VCC         接       DC 5V/3.3V      //OLED屏电源正
//   GND         接          GND          //OLED屏电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为IIC
// OLED模块               STM32单片机
//   SDA         接          PB       //OLED屏IIC数据信号
//=======================================液晶屏控制线接线==========================================//
// OLED模块               STM32单片机
//   SCL         接          PB         //OLED屏IIC时钟信号
//=========================================触摸屏接线=========================================//
****************************************************************************************************/	
/***************************************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
****************************************************************************************************/		
#include "stdlib.h"
#include "stdio.h"
#include "oled.h"
#include "delay.h"
#include "gui.h"
#include "test.h"
#include "bmp.h"
#include "delay.h"
#include <stdlib.h>
#include "os_time_pro.h"
/*****************************************************************************
 * @name       :void TEST_MainPage(void)
 * @date       :2018-08-27 
 * @function   :Drawing the main Interface of the Comprehensive Test Program
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
TTimerItem t1,t2;
u8 t1_count,t2_count;

void TEST_MainPage(void)
{	
	GUI_ShowString(37,0,"OLED TEST",8,1);
	GUI_ShowString(25,8,"0.91\" SSD1306",8,1);
	GUI_ShowString(46,16,"32X128",8,1);
	GUI_ShowString(19,24,"www.lcdwiki.com",8,1);
	delay_ms(1500);		
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Color(void)
 * @date       :2018-08-27 
 * @function   :Color fill test(white,black)
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Color(void)
{
	 GUI_Fill(0,0,WIDTH-1,HEIGHT-1,0);
	 GUI_ShowString(10,10,"BLACK",16,1);
	 delay_ms(1000);	
	 GUI_Fill(0,0,WIDTH-1,HEIGHT-1,1);
	 GUI_ShowString(10,10,"WHITE",16,0);
	 delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Rectangular(void))
 * @date       :2018-08-27
 * @function   :Rectangular display and fill test
								Display black,white rectangular boxes in turn,1000 
								milliseconds later,Fill the rectangle in black,white in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Rectangular(void)
{
	GUI_Fill(0,0,WIDTH/2-1,HEIGHT-1,0);
	GUI_Fill(WIDTH/2,0,WIDTH-1,HEIGHT-1,1);
	GUI_DrawRectangle(5, 5, WIDTH/2-1-5, HEIGHT-1-5,1);
	GUI_DrawRectangle(WIDTH/2-1+5, 5, WIDTH-1-5, HEIGHT-1-5,0);
	delay_ms(1000);
	GUI_FillRectangle(5, 5, WIDTH/2-1-5, HEIGHT-1-5,1);
	GUI_FillRectangle(WIDTH/2-1+5, 5, WIDTH-1-5, HEIGHT-1-5,0);
	delay_ms(1500);
}


/*****************************************************************************
 * @name       :void Test_Circle(void)
 * @date       :2018-08-27 
 * @function   :circular display and fill test
								Display black,white circular boxes in turn,1000 
								milliseconds later,Fill the circular in black,white in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Circle(void)
{
	GUI_Fill(0,0,WIDTH/2-1,HEIGHT-1,0);
	GUI_Fill(WIDTH/2,0,WIDTH-1,HEIGHT-1,1);
	GUI_DrawCircle(WIDTH/2/2-1, HEIGHT/2-1, 1, HEIGHT/2-3);
	GUI_DrawCircle(WIDTH/2+WIDTH/2/2-1, HEIGHT/2-1, 0, HEIGHT/2-3);
	delay_ms(1000);
	GUI_FillCircle(WIDTH/2/2-1, HEIGHT/2-1, 1, HEIGHT/2-3);
	GUI_FillCircle(WIDTH/2+WIDTH/2/2-1, HEIGHT/2-1, 0, HEIGHT/2-3);
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Triangle(void)
 * @date       :2018-08-27 
 * @function   :triangle display and fill test
								Display black,white triangle boxes in turn,1000 
								milliseconds later,Fill the triangle in black,white in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Triangle(void)
{
	GUI_Fill(0,0,WIDTH/2-1,HEIGHT-1,0);
	GUI_Fill(WIDTH/2,0,WIDTH-1,HEIGHT-1,1);
	GUI_DrawTriangel(5,HEIGHT-1-5,WIDTH/2/2-1,5,WIDTH/2-1-5,HEIGHT-1-5,1);
	GUI_DrawTriangel(WIDTH/2-1+5,HEIGHT-1-5,WIDTH/2+WIDTH/2/2-1,5,WIDTH-1-5,HEIGHT-1-5,0);
	delay_ms(1000);
	GUI_FillTriangel(5,HEIGHT-1-5,WIDTH/2/2-1,5,WIDTH/2-1-5,HEIGHT-1-5,1);
	GUI_FillTriangel(WIDTH/2-1+5,HEIGHT-1-5,WIDTH/2+WIDTH/2/2-1,5,WIDTH-1-5,HEIGHT-1-5,0);
	delay_ms(1500);
}


/*****************************************************************************
 * @name       :void TEST_English(void)
 * @date       :2018-08-27 
 * @function   :English display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TEST_English(void)
{
	GUI_ShowString(0,8,"6x8:abcdefghijklmnopqrstuvwxyz",8,1);
	delay_ms(1000);
	GUI_ShowString(0,0,"8x16:abcdefghijklmnopqrstuvwxyz",16,1);
	delay_ms(1000);
	OLED_Clear(0); 
	GUI_ShowString(0,8,"6x8:ABCDEFGHIJKLMNOPQRSTUVWXYZ",8,1);
	delay_ms(1000);
	GUI_ShowString(0,0,"8x16:ABCDEFGHIJKLMNOPQRSTUVWXYZ",16,1);
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void TEST_Number_Character(void) 
 * @date       :2018-08-27 
 * @function   :number and character display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TEST_Number_Character(void) 
{
	GUI_Fill(0,0,WIDTH-1,HEIGHT/2-1,0);
	GUI_ShowString(0,0,"6x8:!\"#$%&'()*+,-./:;<=>?@[]\\^_`~{}|",8,1);
	GUI_ShowNum(68,8,1234567890,10,8,1);
	GUI_Fill(0,HEIGHT/2,WIDTH-1,HEIGHT-1,1);
	GUI_ShowString(0,HEIGHT/2+1,"6x8:!\"#$%&'()*+,-./:;<=>?@[]\\^_`~{}|",8,0);
	GUI_ShowNum(68,HEIGHT/2+9,1234567890,10,8,0);
	delay_ms(1000);
	OLED_Clear(0); 
  GUI_ShowString(0,0,"8x16:!\"#$%&'()*+,-./:;<=>?@[]\\^_`~{}|",16,1);
	GUI_ShowNum(48,16,1234567890,10,16,1);
	delay_ms(1500);
	OLED_Clear(0);
}

/*****************************************************************************
 * @name       :void TEST_Chinese(void)
 * @date       :2018-08-27
 * @function   :chinese display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
u8 data[]={0xC9,0xE6,0xC3,0xDC,0xCE,0xC4,0xBC,0xFE,0xB3,0xF6,0xCF,0xD6,0xA3,0xAC,0xD7,0xBC,0xB1,0xB8,0xB9,0xD8,0xBB,0xFA,0xA1,0xA3};
u8 data3[] ={0xC7,0xEB,0xD2,0xC6,0xBF,0xAA,0xC9,0xE6,0xC3,0xDC,0xCE,0xC4,0xBC,0xFE,0xA3,0xAC,0xB7,0xF1,0xD4,0xF2,0xB8,0xB4,0xD3,0xA1,0xBB,0xFA,0xB9,0xD8,0xBB,0xFA,0xA1,0xA3};
u8 data2[10];
u8 data4[10];
//字体测试
void TEst(u8 count)
{
	u8 i =0;
	for(i=0;i<10;i++)
	{
		data2[i] = data[count];
		count=(count+1)%24;
	}
}
void TEst2(u8 count)
{
	u8 i =0;
	for(i=0;i<10;i++)
	{
		data4[i] = data3[count];
		count=(count+1)%32;
	}
}
//滚动字体,涉密文件出现，准备关机
void TEST_Chinese(void)
{	
	TEst(t1_count);
	OLED_Clear(0);
	GUI_ShowCHinese(0,5,24,data2,1);
	t1_count=t1_count+2;
	if(t1_count==24) 
	{
		t1_count=0;
		os_timer_disarm(&t1);
	}
	os_timer_arm(&t1,500,0,TEST_Chinese);
}
//滚动字体,请移开涉密文件，否则复印机关机
void TEST2_Chinese(void)
{
		TEst2(t2_count);
		OLED_Clear(0);
		GUI_ShowCHinese(0,5,24,data4,1);
	  t2_count=t2_count+2;
	  if(t2_count==32) t2_count=0;
	  os_timer_arm(&t2,500,0,TEST2_Chinese);
}
/*****************************************************************************
 * @name       :void TEST_BMP(void)
 * @date       :2018-08-27 
 * @function   :BMP monochromatic picture display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TEST_BMP(void)
{
	GUI_DrawBMP(0,0,128,32, BMP2, 1);
	delay_ms(1000);
	GUI_DrawBMP(0,0,128,32, BMP2, 0);
	delay_ms(1000);
	GUI_DrawBMP(0,0,128,32, BMP3, 1);
	delay_ms(1000);
	GUI_DrawBMP(0,0,128,32, BMP4, 1);
	delay_ms(1000);
}

/*****************************************************************************
 * @name       :void TEST_Menu(void)
 * @date       :2018-08-27 
 * @function   :English weather interface display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TEST_Menu(void)
{
	u8 i;
	srand(123456789);
	GUI_DrawBMP(0,0,32,32, BMP5, 1);
	GUI_ShowString(38,0,"Steps:",16,1);
	GUI_ShowString(38,16,"Cal:",16,1);
	GUI_ShowString(86,0,"8888",16,1);
	GUI_ShowString(70,16,"888",16,1);
	for(i=0;i<15;i++)
	{
		GUI_ShowNum(86,0,rand()%10+1,1,16,1);
		GUI_ShowNum(94,0,rand()%10,1,16,1);
		GUI_ShowNum(102,0,rand()%10,1,16,1);
		GUI_ShowNum(110,0,rand()%10,1,16,1);
		GUI_ShowNum(70,16,rand()%10+1,1,16,1);
		GUI_ShowNum(78,16,rand()%10,1,16,1);
		GUI_ShowNum(86,16,rand()%10,1,16,1);
    delay_ms(500);	
	}
}



