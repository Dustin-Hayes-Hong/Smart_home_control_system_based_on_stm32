#ifndef __INFRARED_H
#define __INFRARED_H
#include "led.h"
#include "KEY.h"
#include <stdarg.h>  
#include <string.h> 
#include <stdio.h>
#include "usart1.h"
#include "usart3.h"
#define FRAME_START		0x68
#define FRAME_END		0x16
#define MODULE_ADDR		0xff
//红外内码学习指令组包
uint16_t IR_Learn_Pack(uint8_t *data, uint8_t index);
//红外内码发送指令组包
uint16_t IR_Send_Pack(uint8_t *data, uint8_t index);

void usart4_init(u32 baud);
extern u8 Usart4_Receive_Buff[1024];
void key_IR(void);
void usart1_echo(void);//串口一回显
#endif
