#ifndef __DMA_ADC_H
#define __DMA_ADC_H	
#include "sys.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
void  Adc1_Init(void);
void  Adc2_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 
 u16  Get_Adc2(u8 ch); 
u16 Get_Adc2_Average(u8 ch,u8 times); 






// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
/********************ADC1输入通道（引脚）配置**************************/
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOA  
#define    ADC_PORT                      GPIOA

// 注意
// 1-PC0 在霸道里面接的是蜂鸣器，默认被拉低
// 2-PC0 在指南者里面接的是SPI FLASH的 片选，默认被拉高
// 所以 PC0 做 ADC 转换通道的时候，结果可能会有误差

// 转换通道个数
#define    NOFCHANEL										 3

#define    ADC_PIN0                      GPIO_Pin_0
#define    ADC_CHANNEL0                  ADC_Channel_0


#define    ADC_PIN1                      GPIO_Pin_1
#define    ADC_CHANNEL1                  ADC_Channel_1

#define    ADC_PIN6                      GPIO_Pin_6
#define    ADC_CHANNEL6                  ADC_Channel_6



// ADC1 对应 DMA1通道1，ADC3对应DMA2通道5，ADC2没有DMA功能
#define    ADC_x                         ADC1
#define    ADC_DMA_CHANNEL               DMA1_Channel1
#define    ADC_DMA_CLK                   RCC_AHBPeriph_DMA1
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];

/**************************函数声明********************************/
void               ADCx_Init                               (void);
u16 Get_Adc_Average(u8 ch,u8 times);

#endif 
