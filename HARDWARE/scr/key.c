#include "key.h"
#include "stm32f10x_exti.h"
/*-------------------------------------------------*/
/*函数名：初始化KEY按键函数                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;                       //定义一个设置IO的结构体

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);     //使能PA端口时钟

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_4;           //准备设置PB5,PB6,PB7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   		   //下拉输入，默认状态是低电平
    GPIO_Init(GPIOB, &GPIO_InitStructure);            		   //设置PA0
}

/*-------------------------------------------------*/
/*函数名：按键外部中断初始化程序.                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void KEY_Exti_Init(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure; //定义一个设置IO的结构体
    EXTI_InitTypeDef   EXTI_InitStructure; //定义一个设置中断的结构体

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	         //使能GPIO复用功能时钟
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);   //配置KEY1-PB55中断线
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);   //配置KEY1-PB55中断线
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);   //配置KEY2-PB6中断线
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);   //配置KEY3-PB7中断线

    EXTI_InitStructure.EXTI_Line = EXTI_Line4;	                  //准备设置外部5中断线
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //外部中断
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;        //下降沿触发中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能
    EXTI_Init(&EXTI_InitStructure);	 	                          //设置外部中断5线

    EXTI_InitStructure.EXTI_Line = EXTI_Line5;	                  //准备设置外部5中断线
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //外部中断
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;        //下降沿触发中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能
    EXTI_Init(&EXTI_InitStructure);	 	                          //设置外部中断5线

    EXTI_InitStructure.EXTI_Line = EXTI_Line6;	                  //准备设置外部6中断线
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //外部中断
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;        //下降沿触发中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能
    EXTI_Init(&EXTI_InitStructure);	 	                          //设置外部中断6线

    EXTI_InitStructure.EXTI_Line = EXTI_Line7;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //外部中断
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //下降沿触发中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能
    EXTI_Init(&EXTI_InitStructure);	 	                          //设置外部中断7线



    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			  //准备设置外部中断5
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		      //子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				  //使能外部中断1通道
    NVIC_Init(&NVIC_InitStructure);                               //设置外部中断4

    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			  //准备设置外部中断5
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		      //子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				  //使能外部中断1通道
    NVIC_Init(&NVIC_InitStructure);                               //设置外部中断4

}

u8 choose;//选择需要改变的数据
//外部中断4服务程序
void EXTI4_IRQHandler(void)//确认按键
{
    Delay_Ms(10);//消抖
    if(!KEY0_IN_STA)
    {
        if(show_num != 0)
        {
            show_num = 0;
            choose_show = 0;
        }
        else	if(choose_show == 0) show_num = 1;
        else	if(choose_show == 1)
        {
            show_num = 2;
            choose_show = 0;
        }
        else	if(choose_show == 2) show_num = 3;

    }
    EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE5上的中断标志位
}

//外部中断5-6-7服务程序
void EXTI9_5_IRQHandler(void)
{
    Delay_Ms(10);//消抖

    if(KEY1_IN_STA == 0) //消除抖动的影响后，再次判断控制KEY1的IO输入电平，是否是低电平，如果是进入if，说明按键按下了
    {

        if(show_num == 0) //在第一屏幕选择
        {
            choose_show++;
            choose_show %= 3;
            set_show();//显示箭头（选择屏幕）
        }
        if(show_num == 2) //在第二屏幕选择
        {
            choose_show++;
            choose_show %= 3;
            set_show();//显示箭头（选择屏幕）
        }
        if(show_num == 1) //在数据显示的屏幕
            next_data = !next_data;
        if(show_num == 3) //在红外学习显示的屏幕
            next_data = !next_data;
    }
    if(KEY2_IN_STA == 0) //消除抖动的影响后，再次判断控制 KEY2 的IO输入电平，是否是低电平，如果是进入if，说明按键按下了
    {
        if(show_num == 2) //在限值修改显示的屏幕
        {
            if(choose_show == 1)
            {
                if(PPM_MAX < 99)PPM_MAX++;
            }
            else if(choose_show == 0 && co_MAX < 99)co_MAX++;
        }
        if(show_num == 3) //在学习遥控显示的屏幕
        {
            Learn_instruction();//学习遥控的函数
        }

    }
    if(KEY3_IN_STA == 0) //消除抖动的影响后，再次判断控制 KEY3 的IO输入电平，是否是低电平，如果是进入if，说明按键按下了
    {
        if(show_num == 2) //在限值修改显示的屏幕
        {
            if(choose_show == 1)
            {
                if(PPM_MAX > 1)PPM_MAX--;
            }
            else if(choose_show == 0 && co_MAX > 1)co_MAX--;
        }
				if(show_num ==0)
				{
					auto_flag=!auto_flag;
				}
    }
    admin_sub();
    EXTI_ClearITPendingBit(EXTI_Line5);  //清除LINE5上的中断标志位
    EXTI_ClearITPendingBit(EXTI_Line6);  //清除LINE6上的中断标志位
    EXTI_ClearITPendingBit(EXTI_Line7);  //清除LINE7上的中断标志位
}
/***********************************************************
*@fuction	:admin_pub
*@brief		:向服务器上报按键按下后改变的值
*@param		:--
*@return	:void
*@author	:--
*@date		:2022-12-06
***********************************************************/
void admin_sub(void)
{
    char temp[256];             //缓冲区
    sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\""identifier15"\":%d,\""identifier16"\":%d,\""identifier6"\":%d},\"version\":\"1.0.0\"}", (int)PPM_MAX, co_MAX,auto_flag);  //构建数据
    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                //添加数据到发送缓冲区
}
/***********************************************************
*@fuction	:Learn_instruction
*@brief		学习遥控的指令并将学习指令打印到串口一显示
*@param		:红外模块灯灭了说明进入学习指令状态
*@return	:void
*@author	:--
*@date		:2022-12-06
***********************************************************/
void Learn_instruction(void)
{
    if(!Learn_on_ok1)//学第一条指令  //用于判断显示未学习还是已学习
    {
        for(int i = 0; i < 5; i++)
        {
            send_byte(USART3, Learn_IR_ctrl[i]);
            send_byte(USART1, Learn_IR_ctrl[i]);//打印到串口一
        }
        Learn_IR_ctrl[3] += 0x01;
    }
    else if(!Learn_off_ok1)//学第二条指令//用于判断显示未学习还是已学习
    {
        for(int i = 0; i < 5; i++)
        {
            send_byte(USART3, Learn_IR_ctrl[i]);
            send_byte(USART1, Learn_IR_ctrl[i]);//打印到串口一
        }
        Learn_IR_ctrl[3] += 0x01;//将学习通道++ 偏于下一个通道的学习
    }
    else if(!Learn_on_ok2)//学第三条指令//用于判断显示未学习还是已学习
    {
        for(int i = 0; i < 5; i++)
        {
            send_byte(USART3, Learn_IR_ctrl[i]);
            send_byte(USART1, Learn_IR_ctrl[i]);//打印到串口一
        }
        Learn_IR_ctrl[3] += 0x01;//将学习通道++ 偏于下一个通道的学习
    }
    else if(!Learn_off_ok2)//学第四条指令//用于判断显示未学习还是已学习
    {
        for(int i = 0; i < 5; i++)
        {
            send_byte(USART3, Learn_IR_ctrl[i]);
            send_byte(USART1, Learn_IR_ctrl[i]);//打印到串口一
        }
        Learn_IR_ctrl[3] += 0x01;//将学习通道++ 偏于下一个通道的学习
    }
    else if(!Learn_on_ok3)//学第四条指令//用于判断显示未学习还是已学习
    {
        for(int i = 0; i < 5; i++)
        {
            send_byte(USART3, Learn_IR_ctrl[i]);
            send_byte(USART1, Learn_IR_ctrl[i]);//打印到串口一
        }
        Learn_IR_ctrl[3] += 0x01;//将学习通道++ 偏于下一个通道的学习
    }    else if(!Learn_off_ok3)//学第四条指令//用于判断显示未学习还是已学习
    {
        for(int i = 0; i < 5; i++)
        {
            send_byte(USART3, Learn_IR_ctrl[i]);
            send_byte(USART1, Learn_IR_ctrl[i]);//打印到串口一
        }
        Learn_IR_ctrl[3] += 0x01;//将学习通道++ 偏于下一个通道的学习
    }    else if(!Learn_on_ok4)//学第四条指令//用于判断显示未学习还是已学习
    {
        for(int i = 0; i < 5; i++)
        {
            send_byte(USART3, Learn_IR_ctrl[i]);
            send_byte(USART1, Learn_IR_ctrl[i]);//打印到串口一
        }
        Learn_IR_ctrl[3] += 0x01;//将学习通道++ 偏于下一个通道的学习
    }    else if(!Learn_off_ok4)//学第八条指令//用于判断显示未学习还是已学习
    {
        for(int i = 0; i < 5; i++)
        {
            send_byte(USART3, Learn_IR_ctrl[i]);
            send_byte(USART1, Learn_IR_ctrl[i]);//打印到串口一
        }
        Learn_IR_ctrl[3] += 0x01;//将学习通道++ 偏于下一个通道的学习
    }
}


