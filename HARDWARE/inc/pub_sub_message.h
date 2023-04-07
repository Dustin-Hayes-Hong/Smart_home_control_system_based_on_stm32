#ifndef __PUN_SUB_MESSAGE_H
#define __PUN_SUB_MESSAGE_H
#include "stm32f10x.h"  //包含需要的头文件
#include "main.h"       //包含需要的头文件
#include "delay.h"      //包含需要的头文件
#include "usart1.h"     //包含需要的头文件
#include "usart2.h"     //包含需要的头文件
#include "timer3_ping.h"     //包含需要的头文件
#include "led.h"        //包含需要的头文件
#include "key.h"        //包含需要的头文件
#include "motorrun.h"        //包含需要的头文件
extern u8 auto_flag;

#define identifier1 "powerstate"//继电器1
#define identifier2 "powerstate1"//继电器2
#define identifier3 "powerstate2"//继电器3
#define identifier4 "powerstate3"//继电器4
#define identifier5 "powerstate4"//窗帘开关
#define identifier6 "powerstate5"//自动模式
#define identifier7 "powerstate6"//电视
#define identifier8 "powerstate7"//空调2
#define identifier9 "powerstate8"//电视2
#define identifier10 "powerstate9"//烟雾报警
#define identifier11 "powerstate10"//一氧化碳报警
#define identifier12 "VehACSwitch"//空调
#define identifier13 "CO"//一氧化碳
#define identifier14 "PPM"//烟雾
#define identifier15 "PPM_MAX"//烟雾上限
#define identifier16 "CO_MAX"//一氧化碳上限
#define identifier17 "LightLux"//光照
#define identifier18 "tem"//温度
#define identifier19 "hum"//湿度
#define identifier20 "pp"//人数
/*                       状态上报                           */
void Data_State(void);
void init_State(void);
void judge_data_sta(void *str, uint16_t GPIO_Pin);
void data_sta(void * str,int cmd);
/*                       状态上报                           */
#endif
