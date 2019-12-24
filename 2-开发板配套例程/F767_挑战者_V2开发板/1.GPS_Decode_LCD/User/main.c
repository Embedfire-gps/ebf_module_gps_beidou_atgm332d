/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   GPS测试。
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 767 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "main.h"  
#include "stm32f7xx.h"
#include "./led/bsp_led.h" 
#include "./usart/bsp_usart.h"
#include "./key/bsp_key.h" 
#include "./sdram/bsp_sdram.h" 
#include "./lcd/bsp_lcd.h"
#include "./gps/gps_config.h"

/* FatFs includes component */
#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"

extern DMA_HandleTypeDef DMA_Handle;
extern UART_HandleTypeDef GPS_UartHandle;
extern void nmea_decode_test(void);

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	
	/* 系统时钟初始化成216MHz */
  SystemClock_Config();
  
	/* LED 端口初始化 */
	LED_GPIO_Config();	 
  
  //LED_BLUE;
	
	/*LCD端口初始化*/
	LCD_Init();
	
	LCD_LayerInit(0, LCD_FB_START_ADDRESS,ARGB8888);
	
	LCD_DisplayOn(); 
	
	/*选择第0层*/
	LCD_SelectLayer(0);
	
	LCD_Clear(LCD_COLOR_BLACK);  

	/*串口初始化*/
	UARTx_Config();
  
	/*GPS初始化*/
  GPS_Config();

  printf("\r\n野火 GPS模块测试例程\r\n"); 
  
	printf("\r\n本程序对SD卡内的gpslog.txt文件定位日志数据进行解码\r\n"); 
	printf("\r\n若需要对GPS模块串口传回的数据解码，");
	printf("可注释掉gps_config.h文件的宏__GPS_LOG_FILE \r\n"); 
  LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
	
  /* GPS解码测试 */
	HAL_UART_Receive_DMA (&GPS_UartHandle,(uint8_t *)gps_rbuff,GPS_RBUFF_SIZE);
  nmea_decode_test();
  
  while(1);

}

/**
  * @brief  System Clock 配置
  *         system Clock 配置如下 : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  无
  * @retval 无
  */
void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	HAL_StatusTypeDef ret = HAL_OK;

	/* 使能HSE，配置HSE为PLL的时钟源，配置PLL的各种分频因子M N P Q 
	 * PLLCLK = HSE/M*N/P = 25M / 25 *432 / 2 = 216M
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 432;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 9;

	ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
	if(ret != HAL_OK)
	{
		while(1) { ; }
	}

	/* 激活 OverDrive 模式以达到216M频率  */  
	ret = HAL_PWREx_EnableOverDrive();
	if(ret != HAL_OK)
	{
		while(1) { ; }
	}

	/* 选择PLLCLK作为SYSCLK，并配置 HCLK, PCLK1 and PCLK2 的时钟分频因子 
	 * SYSCLK = PLLCLK     = 216M
	 * HCLK   = SYSCLK / 1 = 216M
	 * PCLK2  = SYSCLK / 2 = 108M
	 * PCLK1  = SYSCLK / 4 = 54M
	 */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2; 

	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
	if(ret != HAL_OK)
	{
		while(1) { ; }
	}  
}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

