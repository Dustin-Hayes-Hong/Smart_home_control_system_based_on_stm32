#ifndef __LED_H
#define __LED_H
#include "sys.h"

#define IO_A7_IN_STA   GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_7)  //PA7  控制LED1,读取电平状态，可以判断LED1是点亮还是熄灭
#define IO_A8_IN_STA   GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_8)  //PA8  控制LED2,读取电平状态，可以判断LED2是点亮还是熄灭
#define IO_A12_IN_STA   GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_12) //PA12 控制LED3,读取电平状态，可以判断LED3是点亮还是熄灭
#define IO_A11_IN_STA   GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_11) //PA11 控制LED4,读取电平状态，可以判断LED4是点亮还是熄灭
#define IO_A5_IN_STA   GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) //PA11 控制LED4,读取电平状态，可以判断LED4是点亮还是熄灭

#define BEEP    			PBout(9) //SCL
#define LED1    			PAout(7) //SCL
#define LED2    			PAout(8) //SCL
#define LED3    			PAout(11) //SCL
#define LED4    			PAout(12) //SCL
void LED_Init(void);               //初始化	
void LED_AllOn(void);              //点亮所有LED
void LED_AllOff(void);             //熄灭所有LED

#endif
