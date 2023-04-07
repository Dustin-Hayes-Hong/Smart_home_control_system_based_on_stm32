#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"   
#include "usart1.h"   
//IO方向设置
#define DHT11_IO_IN()  {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=8<<0;}
#define DHT11_IO_OUT() {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=3<<0;}
////IO操作函数											   
#define	DHT11_DQ_OUT PBout(8) //数据端口	PB11 
#define	DHT11_DQ_IN  PBin(8)  //数据端口	PB11 


u8 DHT11_Init(void);//初始化DHT11
u8 DHT11_Read_Data(u8 *humi,u8 *temp)   ;//读取温湿度
u8 DHT11_Read_Byte(void);//读出一个字节
u8 DHT11_Read_Bit(void);//读出一个位
u8 DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11    
#endif






