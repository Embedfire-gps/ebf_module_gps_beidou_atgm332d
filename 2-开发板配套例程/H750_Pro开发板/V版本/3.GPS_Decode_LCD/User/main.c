/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   测试NEMA解码库
  ******************************************************************
  * @attention
  *
  * 实验平台:野火 STM32H750 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************
  */  
#include "stm32h7xx.h"
#include "main.h"
#include "./led/bsp_led.h" 
#include "./usart/bsp_usart.h"
#include "./sdram/bsp_sdram.h" 
#include "./lcd/bsp_lcd.h"
#include "./mpu/bsp_mpu.h" 
#include "string.h"
#include "./gps/gps_config.h"

extern DMA_HandleTypeDef DMA_Handle;
extern UART_HandleTypeDef GPS_UartHandle;
extern void nmea_decode_test(void);
// WIFI和SD卡公用SDIO总线，当使用SD卡时，避免WIFI的冲突
// 需要把WIFI的使能引脚PDN拉低。
void WIFI_PDNPIN_SET(void)
{
   	GPIO_InitTypeDef  GPIO_InitStruct;
   __GPIOB_CLK_ENABLE();														   
    GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,(GPIO_PinState)0);
} 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	
	/* 系统时钟初始化成480MHz */
    SystemClock_Config();
  
	SCB_EnableICache();    // 使能指令 Cache
	SCB_EnableDCache();    // 使能数据 Cache
	
	/* LED 端口初始化 */
	LED_GPIO_Config();	 
  
    LED_BLUE;
	
	/*LCD端口初始化*/
	LCD_Init();
	
	LCD_LayerInit(0, LCD_FB_START_ADDRESS,ARGB8888);
	
	LCD_DisplayOn(); 
	
	LCD_SelectLayer(0);
	
	LCD_Clear(LCD_COLOR_BLACK);  

	/*串口初始化*/
	UARTx_Config();
  
  //GPS_Config();

	// 失能WIFI
	WIFI_PDNPIN_SET();

  printf("\r\n野火 GPS模块测试例程\r\n");   
	printf("\r\n本程序对SD卡内的gpslog.txt文件定位日志数据进行解码\r\n"); 
	printf("\r\n若需要对GPS模块串口传回的数据解码，");
	printf("可注释掉gps_config.h文件的宏__GPS_LOG_FILE \r\n"); 
	
  /* GPS解码测试 */
	HAL_UART_Receive_DMA (&GPS_UartHandle,(uint8_t *)gps_rbuff,GPS_RBUFF_SIZE);
  nmea_decode_test();
  
  while(1);
}

/**
  * @brief  System Clock 配置
  *         system Clock 配置如下: 
	*            System Clock source  = PLL (HSE)
	*            SYSCLK(Hz)           = 480000000 (CPU Clock)
	*            HCLK(Hz)             = 240000000 (AXI and AHBs Clock)
	*            AHB Prescaler        = 2
	*            D1 APB3 Prescaler    = 2 (APB3 Clock  120MHz)
	*            D2 APB1 Prescaler    = 2 (APB1 Clock  120MHz)
	*            D2 APB2 Prescaler    = 2 (APB2 Clock  120MHz)
	*            D3 APB4 Prescaler    = 2 (APB4 Clock  120MHz)
	*            HSE Frequency(Hz)    = 25000000
	*            PLL_M                = 5
	*            PLL_N                = 192
	*            PLL_P                = 2
	*            PLL_Q                = 4
	*            PLL_R                = 2
	*            VDD(V)               = 3.3
	*            Flash Latency(WS)    = 4
  * @param  None
  * @retval None
  */
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** 启用电源配置更新
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** 配置主内稳压器输出电压
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** 初始化CPU、AHB和APB总线时钟
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
		while(1);
  }
  /** 初始化CPU、AHB和APB总线时钟
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
		while(1);
  }
}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/****************************END OF FILE***************************/
