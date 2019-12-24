/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   GPS测试（液晶显示）
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F429 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./Bsp/led/bsp_led.h"
#include "./Bsp/usart/bsp_debug_usart.h"
#include "./Bsp/sdram/bsp_sdram.h"
#include "./Bsp/lcd/bsp_lcd.h"
#include "./gps/gps_config.h"
#include <string.h>



void Delay(__IO u32 nCount); 
void LCD_Test(void);

// WIFI和SD卡公用SDIO总线，当使用SD卡时，避免WIFI的冲突
// 需要把WIFI的使能引脚PDN拉低。
void WIFI_PDNPIN_SET(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
	
		RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOG, ENABLE);															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
		GPIO_Init(GPIOG, &GPIO_InitStructure);	
	
		GPIO_ResetBits(GPIOG,GPIO_Pin_9);
} 

extern void nmea_decode_test(void);

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	/* LED 端口初始化 */
	LED_GPIO_Config();	 
	/*串口初始化*/
	Debug_USART_Config();
  
  /*初始化GPS*/
  GPS_Config(); 

  LED_BLUE;
  
  printf("\r\n野火 GPS模块测试例程\r\n"); 

	// 失能WIFI
	WIFI_PDNPIN_SET();
  
  /*初始化液晶屏*/
  LCD_Init();
  LCD_LayerInit();
  LTDC_Cmd(ENABLE);
	
	/*把背景层刷黑色*/
  LCD_SetLayer(LCD_BACKGROUND_LAYER);  
	LCD_Clear(LCD_COLOR_BLACK);
	
  /*初始化后默认使用前景层*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	/*默认设置不透明	，该函数参数为不透明度，范围 0-0xff ，0为全透明，0xff为不透明*/
  LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_BLACK);
	/*经过LCD_SetLayer(LCD_FOREGROUND_LAYER)函数后，
	以下液晶操作都在前景层刷新，除非重新调用过LCD_SetLayer函数设置背景层*/
			
  Delay(0xfff);

  /*GPS解码*/
  nmea_decode_test();
  
  while(1);

}



void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

