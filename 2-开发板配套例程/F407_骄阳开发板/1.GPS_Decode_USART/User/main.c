/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   对GPS模块传输的数据进行解码，获取定位信息。
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F4 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./gps/gps_config.h"

void Delay(__IO u32 nCount); 

extern void nmea_decode_test(void);

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	/*串口初始化*/
	Debug_USART_Config();
  
  GPS_Config();

  printf("\r\n野火 GPS模块测试例程\r\n"); 
  
	printf("\r\n本程序对GPS模块串口传回的数据解码，");
	printf("实验时请给开发板接入GPS模块 \r\n"); 


  /* GPS解码测试 */
  nmea_decode_test();
  
  while(1);


}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

