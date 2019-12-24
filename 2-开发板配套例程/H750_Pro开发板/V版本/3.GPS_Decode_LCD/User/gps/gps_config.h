#ifndef __GPS_USART_H
#define	__GPS_USART_H

#include "stm32h7xx.h"
#include <stdio.h>
#include "./nmea/nmea.h"

/* GPS接口配置 使用不同的串口时，要修改对应的接口 */

#define GPS_DR_Base            (USART3_BASE+0x04)		  // 串口的数据寄存器地址


#define GPS_DATA_ADDR             GPS_DR_Base        //GPS使用的串口的数据寄存器地址
#define GPS_RBUFF_SIZE            512                  //串口接收缓冲区大小
#define HALF_GPS_RBUFF_SIZE       (GPS_RBUFF_SIZE/2)    //串口接收缓冲区一半  

/* 外设 */
/*******************************************************/
//GPS串口波特率
#define GPS_USART                             USART3
#define GPS_USART_CLK_ENABLE()                __USART3_CLK_ENABLE();

#define GPS_USART_RX_GPIO_PORT                GPIOC
#define GPS_USART_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
#define GPS_USART_RX_PIN                      GPIO_PIN_11
#define GPS_USART_RX_AF                       GPIO_AF7_USART3


#define GPS_USART_TX_GPIO_PORT                GPIOC
#define GPS_USART_TX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
#define GPS_USART_TX_PIN                      GPIO_PIN_10
#define GPS_USART_TX_AF                       GPIO_AF7_USART3

#define GPS_USART_IRQHandler                  USART3_IRQHandler
#define GPS_USART_IRQ                 		    USART3_IRQn
/************************************************************/
//DMA
#define GPS_USART_DMA_CLK_ENABLE()      		  __DMA2_CLK_ENABLE()	
#define GPS_USART_DMA_REQUEST                 DMA_REQUEST_USART3_RX
#define GPS_USART_DMA_STREAM            		  DMA2_Stream7
#define GPS_DMA_IRQHANDLER                    DMA2_Stream7_IRQHandler
#define GPS_DMA_IRQ                 		      DMA2_Stream7_IRQn

/* 外设标志 */
#define GPS_DMA_IT_HT                DMA_IT_HT
#define GPS_DMA_IT_TC                DMA_IT_TC



extern uint8_t gps_rbuff[GPS_RBUFF_SIZE];
extern __IO uint8_t GPS_TransferEnd ;
extern __IO uint8_t GPS_HalfTransferEnd;



void GPS_ProcessDMAIRQ(void);
void GPS_Config(void);

void trace(const char *str, int str_size);
void error(const char *str, int str_size);
void gps_info(const char *str, int str_size);
void GMTconvert(nmeaTIME *SourceTime, nmeaTIME *ConvertTime, uint8_t GMT,uint8_t AREA) ;

#endif /* __USART1_H */
