#include "usart3.h"
#include "stdio.h"      //包含需要的头文件
#include "stdarg.h"		//包含需要的头文件 
#include "string.h"     //包含需要的头文件
void  uart3_init(u32 baud)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART1，GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART1，GPIOA时钟

    USART_DeInit(USART3);  //复位串口1
    //USART1_TX   PB10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PA9

    //USART1_RX	  PB11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PB10

    //Usart1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ; //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = baud;//一般设置为9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART3, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
    USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//开启中断
    USART_Cmd(USART3, ENABLE);                    //使能串口
}

char Usart3_RxCompleted = 0;            //定义一个变量 0：表示接收未完成 1：表示接收完成
unsigned int Usart3_RxCounter = 0;      //定义一个变量，记录串口2总共接收了多少字节的数据
char Usart3_RxBuff[500]; //定义一个数组，用于保存串口2接收到的数据
u8 Usart3_Receive = 0;

/*-------------------------------------------------*/
/*函数名：串口3接收中断函数                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
u8 Learn_on_ok1;
u8 Learn_off_ok1;
u8 Learn_on_ok2;
u8 Learn_off_ok2;
u8 Learn_on_ok3;
u8 Learn_off_ok3;
u8 Learn_on_ok4;
u8 Learn_off_ok4;
void USART3_IRQHandler(void)
{
    if(USART_GetITStatus(USART3, USART_IT_RXNE))
    {
        Usart3_RxBuff[Usart3_RxCounter] = USART_ReceiveData(USART3); //接收数据将数据存到数组
        Usart3_RxCounter++;
        USART_ClearITPendingBit(USART3, USART_IT_RXNE); //清除接收中断标志位
    }
    if(USART_GetITStatus(USART3, USART_IT_IDLE))
    {
        USART3->DR;//清除标志位
        USART3->SR;
        Usart3_RxCounter = 0;
        Usart3_Receive = 1;
        for(int i = 0; i < 5; i++)//将接收到的数据打印出来
            send_byte(USART1, Usart3_RxBuff[i]);
	/*---------------------------------------*/				
        if(!Learn_on_ok1)
        {
            if(Usart3_RxBuff[0] == 0xa2)
                Learn_on_ok1 = 1; //接收到学习空调完成的指令
        }
        else if(!Learn_off_ok1)
				{
					            if(Usart3_RxBuff[0] == 0xa2)
                Learn_off_ok1 = 1; //接收到学习空调完成的指令
				}
	/*---------------------------------------*/			
        else if(!Learn_on_ok2)
				{
					       if(Usart3_RxBuff[0] == 0xa2)
                Learn_on_ok2 = 1; //接收到学习电视完成的指令
				}	
        else if(!Learn_off_ok2)
				{
					       if(Usart3_RxBuff[0] == 0xa2)
                Learn_off_ok2 = 1; //接收到学习电视完成的指令
				}	
	/*---------------------------------------*/			
				   else if(!Learn_on_ok3)
				{
					       if(Usart3_RxBuff[0] == 0xa2)
                Learn_on_ok3 = 1; //接收到学习空调2完成的指令
				}					
        else if(!Learn_off_ok3)
				{
					       if(Usart3_RxBuff[0] == 0xa2)
                Learn_off_ok3 = 1; //接收到学习空调2完成的指令
				}	

		/*---------------------------------------*/					
							        else if(!Learn_on_ok4)
				{
					       if(Usart3_RxBuff[0] == 0xa2)
                Learn_on_ok4 = 1; //接收到学习电视2完成的指令
				}				
				else if(!Learn_off_ok4)
				{
					       if(Usart3_RxBuff[0] == 0xa2)
                Learn_off_ok4 = 1; //接收到学习电视2完成的指令
				}	
	
					/*---------------------------------------*/		
			 memset(Usart3_RxBuff, 0, sizeof(Usart3_RxBuff));			
    }
}


