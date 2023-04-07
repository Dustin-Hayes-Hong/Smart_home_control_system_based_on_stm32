#include "stm32f10x.h"  //包含需要的头文件
#include "motorrun.h"        //包含需要的头文件

/*接线：
				步进电机				单片机
					A相		------	P1^0
					AA相		------	P1^1
					B相		------	P1^2
					BB相		------	P1^3
*/

void MOTOR_Init()
{
  GPIO_InitTypeDef GPIO_MOTOR;                     //定义一个设置GPIO的变量
                             //使能GPIOA端口时钟
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);                                //使能GPIOB端口时钟
	

	GPIO_MOTOR.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_1|GPIO_Pin_0 ;                //准备设置PB12 PB13 PB14
	GPIO_MOTOR.GPIO_Speed = GPIO_Speed_50MHz;                                     //IO速率50Mhz
	GPIO_MOTOR.GPIO_Mode = GPIO_Mode_Out_PP;   		                              //推免输出方式
	GPIO_Init(GPIOB, &GPIO_MOTOR);   
  	
}

void Delay10us()		//@11.0592MHz
{
	unsigned char i;

	i = 2;
	while (--i);
}

void delay_10us(unsigned int  time)//延时time*10 us
{
	while(time--)
	{Delay10us();}
}

/*
angle：角度，范围[0,360]
drct：旋转方向，0：顺时针，1：逆时针
speed：转速，范围[1,100]
*/
u8 MotorRun_flag;//用于显示窗帘的打开和关闭
void MotorRun(unsigned int nangle,unsigned int drct,unsigned int  speed)					//nangle=4096为一圈						 speed转速75`400
{
	unsigned int  step;
	step=0;
	if(drct==0)
	{
		MotorRun_flag=1;
		while(nangle--)
		{
			
			if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr){                    //if成立的话，说明发送缓冲区有数据了
				//3种情况可进入if
				//第1种：0x10 连接报文
				//第2种：0x82 订阅报文，且ConnectPack_flag置位，表示连接报文成功
				//第3种：SubcribePack_flag置位，说明连接和订阅均成功，其他报文可发
				if((DMA_flag==0)&&((MQTT_TxDataOutPtr[2]==0x10)||((MQTT_TxDataOutPtr[2]==0x82)&&(ConnectPack_flag==1))||(SubcribePack_flag==1))){    
					if(MQTT_TxDataOutPtr[2]==0xc0)
					printf("发送ping数据:0x%x\r\n",MQTT_TxDataOutPtr[2]);  //串口提示信息
					else 					printf("发送数据:0x%x\r\n",MQTT_TxDataOutPtr[2]);  //串口提示信息
					MQTT_TxData(MQTT_TxDataOutPtr);                       //发送数据
					MQTT_TxDataOutPtr += TBUFF_UNIT;                      //指针下移
					if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)              //如果指针到缓冲区尾部了
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];            //指针归位到缓冲区开头
				} 				
			}//处理发送缓冲区数据的else if分支结尾
			switch(step)//8拍方式驱动
			{
				case 0:A=1;AA=0;B=0;BB=0;break;
				case 1:A=1;AA=1;B=0;BB=0;break;
				case 2:A=0;AA=1;B=0;BB=0;break;
				case 3:A=0;AA=1;B=1;BB=0;break;
				case 4:A=0;AA=0;B=1;BB=0;break;
				case 5:A=0;AA=0;B=1;BB=1;break;
				case 6:A=0;AA=0;B=0;BB=1;break;
				case 7:A=1;AA=0;B=0;BB=1;break;
			}
			if(step==7)step=0;
			else step++;
			delay_10us(speed);
		}
	}
	else
	{
			MotorRun_flag=0;
		while(nangle--)
		{
			switch(step)
			{
				case 0:A=1;AA=0;B=0;BB=1;break;
				case 1:A=0;AA=0;B=0;BB=1;break;
				case 2:A=0;AA=0;B=1;BB=1;break;
				case 3:A=0;AA=0;B=1;BB=0;break;
				case 4:A=0;AA=1;B=1;BB=0;break;
				case 5:A=0;AA=1;B=0;BB=0;break;
				case 6:A=1;AA=1;B=0;BB=0;break;
				case 7:A=1;AA=0;B=0;BB=0;break;
			}
			if(step==7)step=0;
			else step++;
			delay_10us(speed);
		}
	}
	A=0;AA=0;B=0;BB=0;
						printf("%MotorRun_flag %d\r\n",MotorRun_flag);
}

void MotorStop() //步进停止
{
	A=0;AA=0;B=0;BB=0;
}


