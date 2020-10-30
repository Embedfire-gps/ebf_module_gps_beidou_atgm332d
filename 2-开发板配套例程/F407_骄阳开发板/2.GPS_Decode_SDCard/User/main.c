/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   对SD卡内的GPS数据进行解码，获取定位信息。
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
#include "key/bsp_key.h" 
#include "ff.h"
#include "./USBAPP/usbh_bsp.h"
/**
  ******************************************************************************
  *                              定义变量
  ******************************************************************************
  */
DIR dir; 
FIL fnew;													/* 文件对象 */
UINT fnum;            			  /* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
BYTE WriteBuffer[] =              /* 写缓冲区*/
"欢迎使用野火STM32 F407骄阳开发板 今天是个好日子，新建文件系统测试文件\r\n";  
char SDPath[4]; /* SD逻辑驱动器路径 */
FATFS sd_fs[2];
FRESULT res_sd;                /* 文件操作结果 */
extern char src_dir[];

uint8_t state = 0;

void Delay(__IO u32 nCount); 
extern void nmea_decode_test(void);
 
#define Other_Part   0
PARTITION VolToPart[]=
{
	{0,1},/* "0:" */
	{0,2},/* "1:" */
	{0,3}	/* "2:" */
};


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
	printf("\r\n本程序对u盘卡内的gpslog.txt文件定位日志数据进行解码\r\n"); 
	printf("\r\n若需要对GPS模块串口传回的数据解码，");
	printf("可注释掉gps_config.h文件的宏__GPS_LOG_FILE \r\n"); 
	
#if __GPS_LOG_FILE  
 		/* 初始化USB */
	USBH_Init(&USB_OTG_Core,
		        USB_OTG_FS_CORE_ID,
		        &USB_Host,
		        &USBH_MSC_cb,
		        &USR_cb);
	
	printf("插入U盘后，提示“Device Attached”字样，表示U盘已检测到，按下KEY1继续操作\n");  
	while(1)
	{
		/* USB状态检测处理函数，在检测设备插入时使用 */
		USBH_Process(&USB_OTG_Core, &USB_Host);
		 
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON  )
		{
			//在外部U盘挂载文件系统，文件系统挂载时会对U盘初始化
			res_sd = f_mount(&sd_fs[0],"0:",1);  
			if(res_sd != FR_OK)
			{
			  printf("f_mount 1ERROR!请给开发板插入U盘然后重新复位开发板!");
			}

#if Other_Part
			res_sd = f_mount(&sd_fs[1],"1:",1);
		  
			if(res_sd != FR_OK)
			{
			  printf("f_mount 2ERROR!请给开发板插入U盘然后重新复位开发板!");
			}
#endif
	    /* GPS解码测试 */
      nmea_decode_test();	
		}
	}	

#else
	/* GPS解码测试 */
  nmea_decode_test();	
#endif
}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

