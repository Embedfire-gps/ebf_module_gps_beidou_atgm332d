#ifndef __GPS_USART_H
#define	__GPS_USART_H

#include "stm32f7xx.h"
#include <stdio.h>
#include "./nmea/nmea.h"


//引脚定义
/*******************************************************/
#define GPS_USART                             USART6
#define GPS_USART_CLK_ENABLE()                __USART6_CLK_ENABLE();

#define GPS_USART_RX_GPIO_PORT                GPIOC
#define GPS_USART_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
#define GPS_USART_RX_PIN                      GPIO_PIN_7
#define GPS_USART_RX_AF                       GPIO_AF8_USART6


#define GPS_USART_TX_GPIO_PORT                GPIOC
#define GPS_USART_TX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
#define GPS_USART_TX_PIN                      GPIO_PIN_6
#define GPS_USART_TX_AF                       GPIO_AF8_USART6

#define GPS_USART_IRQHandler                  USART6_IRQHandler
#define GPS_USART_IRQ                 		    USART6_IRQn
/************************************************************/
//DMA
#define GPS_USART_DMA_CLK_ENABLE()      		  __DMA2_CLK_ENABLE()	
#define GPS_DMA_CHANNEL                       DMA_CHANNEL_5
#define GPS_USART_DMA_STREAM            		  DMA2_Stream2
#define GPS_DMA_IRQHANDLER                    DMA2_Stream2_IRQHandler
#define GPS_DMA_IRQ                 		      DMA2_Stream2_IRQn




#define GPS_RBUFF_SIZE            256                   //串口接收缓冲区大小
#define HALF_GPS_RBUFF_SIZE       (GPS_RBUFF_SIZE/2)    //串口接收缓冲区一半  

extern __IO uint8_t GPS_TransferEnd ;
extern __IO uint8_t GPS_HalfTransferEnd;
extern uint8_t gps_rbuff[GPS_RBUFF_SIZE];

//串口波特率
#define GPS_USART_BAUDRATE                    9600



void GPS_Config(void);
void GPS_ProcessDMAIRQ(void);
void GPS_USART_Config(void);
void USART_DMA_Config(void);
void trace(const char *str, int str_size);
void error(const char *str, int str_size);
void gps_info(const char *str, int str_size);
void GMTconvert(nmeaTIME *SourceTime, nmeaTIME *ConvertTime, uint8_t GMT,uint8_t AREA) ;
//int fputc(int ch, FILE *f);
extern UART_HandleTypeDef GPS_UartHandle;


#endif /*__GPS_USART_H */
