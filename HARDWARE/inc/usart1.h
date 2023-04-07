#ifndef __USART1_H
#define __USART1_H
#include "total_wifi.h"
#include "stdio.h"      //包含需要的头文件
#include "stdarg.h"		//包含需要的头文件 
#include "string.h"     //包含需要的头文件
#define USART1_RX_ENABLE  1
void Usart1_Init(unsigned int);     //串口1 初始化函数
void send_str_len(USART_TypeDef* USARTx,u8 *str,u8 len);
extern char Usart1_Rx_ok ;            //定义一个变量 0：表示接收未完成 1：表示接收完成
extern u8 Usart_Receive_Buff[255];
void voice_ctrl(void);
void send_byte(USART_TypeDef *USARTx, u8 byte);
extern int IR_ctrl_on[5]; 
extern int IR_ctrl_off[5] ; 

#endif


