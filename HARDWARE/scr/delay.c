
#include "stm32f10x.h"    //包含需要的头文件


/*-------------------------------------------------*/
/*函数名：初始化延迟计数器函数                     */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Delay_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	//SysTick_CLKSource_HCLK_Div8 = 8 表示分频系数
	//因为分频系数我们设置为8，所以SysTick（滴答时钟）的频率是主频率的1/8
	//通常主频率为最大的72M，那么SysTick的频率是9M
	//那么SysTick计数器的1个数，代表(1/9)us
}

/*-------------------------------------------------*/
/*函数名：延迟微秒函数                             */
/*参  数：us：延时多少微秒                         */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Delay_Us(unsigned int us)
{		
	unsigned int temp;	                      //定义一个变量待用    	 
	
	SysTick->LOAD=us*9;                       //计数器的重载值，要注意SysTick是倒数计数的
	                                          //SysTick计数器每倒数一个数是1/9微秒，所以我们用us*9，就是计数器的重载值
	SysTick->VAL=0x00;                        //清空当前计数器的值
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;  //使能SysTick计数器，重载值加载到计数器中，开始倒数计数
	do{
		temp=SysTick->CTRL;                   //循环读取SysTick状态寄存器，用于判断计时结束与否
	}while(temp&0x01&&!(temp&(1<<16)));       //等待时间到达
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;  //关闭计数器 
}

/*-------------------------------------------------*/
/*函数名：延迟毫秒函数                             */
/*参  数：ms：延时多少毫秒                         */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Delay_Ms(unsigned int ms)
{
	//我们首先注意一个问题SysTick时钟计数器是24位的，9M频率下，总共能延时1864.135ms
	//所有我们以1800为界限，小于1800的延时一次计数就行，大于1800的多次计数	
	
	unsigned char i;      //定义一个变量待用  
	unsigned int temp;    //定义一个变量待用  
	
	/*-----------if判断，小于1800ms延时的情况，执行if分支------------------*/
	if(ms<1800){                                   
		SysTick->LOAD=(unsigned int)ms*9*1000;     //计数器的重载值，要注意SysTick是倒数计数的        
		                                           //SysTick计1个数是1/9微秒，换算成ms的话，乘以9再乘以1000，就是计数器的重载值
		SysTick->VAL=0x00;                         //清空当前计数器的值
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;   //使能SysTick计数器，重载值加载到计数器中，开始倒数
		do{
			temp=SysTick->CTRL;                    //循环读取SysTick状态寄存器，判断计时结束与否
		}while(temp&0x01&&!(temp&(1<<16)));        //等待时间到达
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;}  //关闭计数器 
	
	/*--------------大于1800ms延时的情况，执行else分支-------------------*/
	else{ 
		for(i=0;i<(ms/1800);i++){                     //除以1800，整数部分，用for循环，每次延时1800ms
			SysTick->LOAD=(unsigned int)1800*9*1000;  //计数器的重载值，要注意SysTick是倒数计数的        
													  //SysTick一个数是1/9微秒，1800ms就是 1800*9*1000
			SysTick->VAL=0x00;                        //清空当前计数器的值
			SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;  //使能SysTick计数器，重载值加载到计数器中，开始倒数
			do{
				temp=SysTick->CTRL;                   //循环读取SysTick状态寄存器，判断计时结束与否
			}while(temp&0x01&&!(temp&(1<<16)));       //等待时间到达
			SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;} //关闭计数器 
		
		//接下来余数部分的延时
		SysTick->LOAD=(unsigned int)(ms%1800)*9*1000; //计数器的重载值，要注意SysTick是倒数计数的  
		                                              //SysTick一个数是1/9微秒，余数部分就是(ms%1800)*9*1000
		SysTick->VAL =0x00;                           //清空当前计数器的值
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;      //使能SysTick计数器，重载值加载到计数器中，开始倒数  
		do{
			temp=SysTick->CTRL;                       //循环读取SysTick状态寄存器，判断计时结束与否
		}while(temp&0x01&&!(temp&(1<<16)));           //等待时间到达
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;      //关闭计数器
	}
} 
