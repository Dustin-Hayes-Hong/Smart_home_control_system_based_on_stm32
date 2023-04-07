#ifndef __USART3_H
#define __USART3_H	 
#include "sys.h"
#include "show.h"
#include "usart1.h"

void  uart3_init(u32 baud);
extern char Usart3_RxCompleted ;            //定义一个变量 0：表示接收未完成 1：表示接收完成
extern unsigned int Usart3_RxCounter ;      //定义一个变量，记录串口2总共接收了多少字节的数据
extern char Usart3_RxBuff[500]; //定义一个数组，用于保存串口2接收到的数据
extern u8 Usart3_Receive;

extern u8 Learn_on_ok1;
extern u8 Learn_off_ok1;
extern u8 Learn_on_ok2;
extern u8 Learn_off_ok2;

extern u8 Learn_on_ok3;
extern u8 Learn_off_ok3;
extern u8 Learn_on_ok4;
extern u8 Learn_off_ok4;

extern int Learn_IR_ctrl_on[5];
extern int Learn_IR_ctrl_off[5];
void u3_printf(char* fmt,...) ;		
void  Learn_instruction(void);
#endif
