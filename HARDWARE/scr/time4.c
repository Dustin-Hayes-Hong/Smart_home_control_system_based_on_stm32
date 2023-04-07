#include "time4.h"
/*-------------------------------------------------*/
/*函数名：定时器4使能10uS定时                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM4_ENABLE_10uS(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //定义一个设置定时器的变量
	NVIC_InitTypeDef NVIC_InitStructure;                          //定义一个设置中断的变量
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);           //使能TIM2时钟	
	TIM_DeInit(TIM4);                                             //定时器2寄存器恢复默认值	
	TIM_TimeBaseInitStructure.TIM_Period = 1; 	              //设置自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=360;              //设置定时器预分频数
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);            //设置TIM2
	
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);                    //清除溢出中断标志位
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);                      //使能TIM2溢出中断    
	TIM_Cmd(TIM4,ENABLE);                                         //开TIM2                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;                 //设置TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                               //设置中断
}

/*-------------------------------------------------*/
/*函数名：定时器2中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM4_IRQHandler(void)
{	

	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){  //如果TIM_IT_Update置位，表示TIM2溢出中断，进入if	

		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     //清除TIM2溢出中断标志 	
	}
}



