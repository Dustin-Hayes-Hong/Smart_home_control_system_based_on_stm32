#include "stm32f10x.h"  //包含需要的头文件
#include "led.h"        //包含需要的头文件

/*-------------------------------------------------*/
/*函数名：初始化LED函数                            */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;                     //定义一个设置GPIO的变量

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);                                //使能GPIOA端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_5;                                     //准备设置PA5
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                     //IO速率50Mhz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   		                              //开楼输出方式
    GPIO_Init(GPIOA, &GPIO_InitStructure);            		                              //设置PA5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOB, &GPIO_InitStructure);            		                              //设置PA5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   		                              //开楼输出方式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;                                     //IO速率50Mhz
    GPIO_Init(GPIOA, &GPIO_InitStructure);            		                              //设置PA5
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    BEEP = 1;

}





