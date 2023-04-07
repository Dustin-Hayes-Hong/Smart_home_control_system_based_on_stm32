#ifndef __SHOW_H
#define __SHOW_H
#include "sys.h"
#include "oled.h"
#include "led.h"        //包含需要的头文件
#include "usart3.h"        //包含需要的头文件
#include "motorrun.h"        //包含需要的头文件
void	Cut_screen(void);
void  main_sreen(void);//主屏幕
void data_show(void);
void show_Learn_instruction(void);//学习指令显示函数
void set_show(void); //选择屏幕
void show_admin(void);
	void show_beep(void);

extern u8 light,ppm,co;
extern u16 adc_light,adc_ppm,adc_co;
extern u8 tem,hum,people;
extern char show_num;//屏幕号
extern char next_data;//显示数据的第二屏幕
extern unsigned char choose_show;//选择显示的屏幕
extern unsigned char choose_admin;//选择修改的数据的屏幕
extern u8 PPM_MAX,co_MAX;
extern u8 ppm_beep,ppm_beep_sta;
extern u8 co_beep,co_beep_sta;


#endif
