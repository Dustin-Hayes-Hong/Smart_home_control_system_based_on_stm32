
#include "stm32f10x.h"  //包含需要的头文件
#include "usart1.h"     //包含需要的头文件

#if  USART1_RX_ENABLE                   //如果使能接收功能

unsigned int Usart1_RxCounter = 0;      //定义一个变量，记录串口1总共接收了多少字节的数据
char Usart1_RxBuff[255]; //定义一个数组，用于保存串口1接收到的数据
#endif
char Usart1_Rx_ok = 0;            //定义一个变量 0：表示接收未完成 1：表示接收完成
/*-------------------------------------------------*/
/*函数名：初始化串口1发送功能                      */
/*参  数：bound：波特率                            */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Usart1_Init(unsigned int bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;     //定义一个设置GPIO功能的变量
    USART_InitTypeDef USART_InitStructure;   //定义一个设置串口功能的变量
#if USART1_RX_ENABLE                         //如果使能接收功能
    NVIC_InitTypeDef NVIC_InitStructure;     //如果使能接收功能，定义一个设置中断的变量
#endif

#if USART1_RX_ENABLE                                 //如果使能接收功能
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3
#endif

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //使能串口1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;              //准备设置PA9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO速率50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //复用推挽输出，用于串口1的发送
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //设置PA9

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;             //准备设置PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入，用于串口1的接收
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //设置PA10

    USART_InitStructure.USART_BaudRate = bound;                                    //波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8个数据位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
#if USART1_RX_ENABLE               												   //如果使能接收模式
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //收发模式
#else                                                                              //如果不使能接收模式
    USART_InitStructure.USART_Mode = USART_Mode_Tx ;	                           //只发模式
#endif
    USART_Init(USART1, &USART_InitStructure);                                      //设置串口1

#if USART1_RX_ENABLE  	         					        //如果使能接收模式
    USART_ClearFlag(USART1, USART_FLAG_RXNE);	            //清除接收标志位
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);          //开启接收中断
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);          //开启接收中断
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;       //设置串口1中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//中断通道使能
    NVIC_Init(&NVIC_InitStructure);	                        //设置串口1中断
#endif

    USART_Cmd(USART1, ENABLE);                              //使能串口1
}
char Usart_buff[1024];
int fputc(int ch, FILE *p) //函数默认的，在使用printf函数时自动调用
{
    USART_SendData(USART1, (u8)ch);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    return ch;
}

/***************************************************************************
* 函 数 名：void send_byte(USART_TypeDef* USARTx,u8 byte)
* 功能说明：串口1发送一个字符
* 形    参：USARTx：使用的串口，byte：发送的字符
* 返 回 值：无
* 备    注：2022-9-21
****************************************************************************/
void send_byte(USART_TypeDef *USARTx, u8 byte)
{
    while(!(USART_GetFlagStatus(USARTx, USART_FLAG_TC))); //疑问--？tc-发送完成和txe发送寄存器为空

    USART_SendData(USARTx, byte);
}
/***************************************************************************
* 函 数 名：void send_str(USART_TypeDef* USARTx,u8 *str)
* 功能说明：串口1发送一串字符
* 形    参：USARTx：使用的串口，*str：发送的字符串
* 返 回 值：无
* 备    注：2022-9-21
****************************************************************************/
void send_str(USART_TypeDef *USARTx, u8 *str)
{
    int len = 0;
    len = strlen((const char *)str);

    while(len--)
        send_byte(USARTx, *str++);
}
/***************************************************************************
* 函 数 名：void send_str(USART_TypeDef* USARTx,u8 *str)
* 功能说明：串口1发送一串字符
* 形    参：USARTx：使用的串口，*str：发送的字符串,u8 len  发送的长度
* 返 回 值：无
* 备    注：2022-9-21
****************************************************************************/
void send_str_len(USART_TypeDef *USARTx, u8 *str, u8 len)
{
    while(len--)
        send_byte(USARTx, *str++);
}
/***************************************************************************
* 函 数 名：void my_printf(USART_TypeDef* USARTx,char *fmt,...)
* 功能说明：串口格式化输出
* 形    参：USARTx：使用的串口，char *fmt：%%%,...
* 返 回 值：无
* 备    注：2022-9-21
****************************************************************************/
void my_printf(USART_TypeDef *USARTx, char *fmt, ...) //疑问？ap怎么移动获取到参数
{
    va_list ap;//用来获取参数列表中的参数
    va_start(ap, fmt); //用来将初始化ap，指向fmt
    vsprintf(Usart_buff, fmt, ap); // 把参数ap按照fmt指定的格式写入buff
    va_end(ap);//结束ap

    send_str(USARTx, (u8 *)Usart_buff);
    memset(Usart_buff, 0, sizeof(Usart_buff));
}
u8 USART_Receive_byte(USART_TypeDef *USARTx, u8 byte)
{
    while(!USART_GetFlagStatus(USARTx, USART_FLAG_RXNE)); //USART_FLAG_RXNE置一时，接收数据寄存器有数据
    return	USART_ReceiveData(USARTx);//返回接收到的数据
}
u8 Usart_Receive_Buff[255];
u8 USART_Receive_str(USART_TypeDef *USARTx, u8 *str)
{
    int i = 0;

    if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)) //USART_FLAG_RXNE置一时，接收数据寄存器有数据
    {
        Usart_Receive_Buff[i] = USART_Receive_byte(USARTx, *str++);
    }
    return 0;

}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE))
    {
        Usart_Receive_Buff[Usart1_RxCounter++] = USART_ReceiveData(USART1);//保存接受到的数据
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
    if(USART_GetITStatus(USART1, USART_IT_IDLE))
    {
        USART1->SR;
        USART1->DR;
        Usart1_Rx_ok = 1;//表示接受完成
        Usart1_RxCounter = 0;

    }
}
void voice_ctrl(void)//语音控制函数
{
    if(Usart1_Rx_ok)//接受到数据
    {
        if(strstr((const char *)Usart_Receive_Buff, "LEDON")) //窗帘打开的指令
        {
					  data_sta("powerstate4",1);
            MotorRun(2048, 0, 5000);
					
        }
        else if(strstr((const char *)Usart_Receive_Buff, "LEDOFF")) //窗帘关闭的指令
        {
					  data_sta("powerstate4",0);
            MotorRun(2048, 1, 5000);
        }
        else if(strstr((const char *)Usart_Receive_Buff, "LED1ON")) //继电器1打开的指令
        {
									  data_sta("powerstate",1);
            LED1 = 0;
						
        }
        else if(strstr((const char *)Usart_Receive_Buff, "LED1OFF")) //继电器1关闭的指令
        {
									  data_sta("powerstate",0);
            LED1 = 1;
						
        }
        else if(strstr((const char *)Usart_Receive_Buff, "LED2ON"))  //继电器2打开的指令
        {
									  data_sta("powerstate1",1);
            LED2 = 0;
										
        }
        else if(strstr((const char *)Usart_Receive_Buff, "LED2OFF")) //继电器2关闭的指令
        {
									  data_sta("powerstate1",0);
            LED2 = 1;	
        }
        else if(strstr((const char *)Usart_Receive_Buff, "LED3ON"))  //继电器3打开的指令
        {
									  data_sta("powerstate2",1);
            LED3 = 0;
        }
        else if(strstr((const char *)Usart_Receive_Buff, "LED3OFF"))  //继电器3关闭的指令
        {
									  data_sta("powerstate2",0);
            LED3 = 1;
        }

        else if(strstr((const char *)Usart_Receive_Buff, "LED4ON"))  //继电器4打开的指令
        {								
					data_sta("powerstate3",1);
            LED4 = 0;
        }
        else if(strstr((const char *)Usart_Receive_Buff, "LED4OFF"))  //继电器4关闭的指令
        {									  data_sta("powerstate3",0);
            LED4 = 1;
        }
        else if(strstr((const char *)Usart_Receive_Buff, "LED5ON"))  //遥控的指令
        {
						IR_ctrl_on[3]=0x00;
            for(int i = 0; i < 5; i++)
            {
                send_byte(USART3, IR_ctrl_on[i]);
            }
						
					 data_sta("VehACSwitch",1);
        }
        else if(strstr((const char *)Usart_Receive_Buff, "LED5OFF"))  //遥控的指令
        {
						IR_ctrl_off[3]=0x01;
            for(int i = 0; i < 5; i++)
            {
                send_byte(USART3, IR_ctrl_off[i]);
                send_byte(USART1, IR_ctrl_off[i]);
            }
					 data_sta("VehACSwitch",0);
        }
        else if(strstr((const char *)Usart_Receive_Buff, "LED6ON"))  //遥控的指令
        {
					IR_ctrl_on[3]=0x02;
            for(int i = 0; i < 5; i++)
            {
                send_byte(USART3, IR_ctrl_on[i]);
                send_byte(USART1, IR_ctrl_on[i]);
            }

        }
        else if(strstr((const char *)Usart_Receive_Buff, "LED6OFF")) //遥控的指令
        {
					IR_ctrl_off[3]=0x03;
            for(int i = 0; i < 5; i++)
            {
                send_byte(USART3, IR_ctrl_off[i]);
                send_byte(USART1, IR_ctrl_off[i]);
            }

        }
				        else if(strstr((const char *)Usart_Receive_Buff, "LED7ON"))  //遥控的指令
        {
					IR_ctrl_on[3]=0x04;
            for(int i = 0; i < 5; i++)
            {
                send_byte(USART3, IR_ctrl_on[i]);
                send_byte(USART1, IR_ctrl_on[i]);
            }

        }
        else if(strstr((const char *)Usart_Receive_Buff, "LED7OFF")) //遥控的指令
        {
					IR_ctrl_off[3]=0x05;
            for(int i = 0; i < 5; i++)
            {
                send_byte(USART3, IR_ctrl_off[i]);
                send_byte(USART1, IR_ctrl_off[i]);
            }

        }        else if(strstr((const char *)Usart_Receive_Buff, "LED8ON"))  //遥控的指令
        {
					IR_ctrl_on[3]=0x06;
            for(int i = 0; i < 5; i++)
            {
                send_byte(USART3, IR_ctrl_on[i]);
                send_byte(USART1, IR_ctrl_on[i]);
            }

        }
        else if(strstr((const char *)Usart_Receive_Buff, "LED8OFF"))  //遥控的指令
        {
					IR_ctrl_off[3]=0x07;
            for(int i = 0; i < 5; i++)
            {
                send_byte(USART3, IR_ctrl_off[i]);
                send_byte(USART1, IR_ctrl_off[i]);
            }

        }
        Usart1_Rx_ok = 0;
        memset(Usart_Receive_Buff, 0, sizeof(Usart_Receive_Buff));
    }
}







