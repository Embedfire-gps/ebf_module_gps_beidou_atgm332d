/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   对GPS模块传输的数据进行解码，获取定位信息。
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F103-霸道 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */

#include "stm32f10x.h"
#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h"
#include "./gps/gps_config.h"
#include "./lcd/bsp_ili9341_lcd.h"

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
  
  LED_BLUE;
  
	ILI9341_Init();
	/*串口初始化*/
	USART_Config();
  
  GPS_Config();

  printf("\r\n野火 GPS模块测试例程\r\n"); 
  

  /* GPS解码测试 */
  nmea_decode_test();
  
  while(1);


}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
