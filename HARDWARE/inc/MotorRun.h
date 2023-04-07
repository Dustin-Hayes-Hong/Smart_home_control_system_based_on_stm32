#ifndef __MOTORRUN_H_
#define __MOTORRUN_H_
#include "SYS.h"
#include "pub_sub_message.h"
#include "total_wifi.h"
#include "time4.h"
#define A PBout(0)
#define AA PBout(14)
#define B PBout(15)
#define BB PBout(1)
void MOTOR_Init(void);
void delay_10us(unsigned int  time);//延时time*10 us
void MotorRun(unsigned int nangle,unsigned int drct,unsigned int  speed)			;	//	nangle=4096为一圈；	drct=0为逆时针转动，drct=1为顺时针转动；  speed转速延时750`4000us；
void MotorStop(void);
extern unsigned int nangle;
extern u8 MotorRun_flag;
#endif
