#include "pub_sub_message.h"
#include "total_wifi.h"
char *star_sp, *end_sp; //用于提取标识符
char identifier_temp[50];//用于存放标识符
int identifier_data;//用于存放下发的数据

void  publish_message(void)
{
    //	命令:{"method":"thing.service.property.set","id":"2092800572","params":{"CO_MAX":53},"version":"1.0.0"}
    printf("命令:%s\r\n", &MQTT_CMDOutPtr[2]);                //串口输出信息
    if(star_sp = strstr((char *)MQTT_CMDOutPtr + 2, (char *)"params")) //定位得到标识符前面的字符 ，用于求出标识符有多大
    {
        end_sp = strstr(star_sp + 10, "\":"); //定位得到标识符后面的字符 ，用于求出标识符有多大
        memcpy(identifier_temp, star_sp + 10, (end_sp) - (star_sp + 10)); //将标识符提取
        identifier_data = atoi((const char *)end_sp + 2); //得到下发的数据
        if(!(strcmp(identifier_temp, ""identifier1""))) //继电器1
        {
            judge_data_sta(""identifier1"", GPIO_Pin_7); //判断电平状态并发布给app
            LED1 = !identifier_data;//控制继电器
        }
        else if(!(strcmp(identifier_temp, ""identifier2"")))//继电器二
        {
            judge_data_sta(""identifier2"", GPIO_Pin_8); //判断电平状态并发布给app
            LED2 = !identifier_data;//控制继电器
        }
        else if(!(strcmp(identifier_temp, ""identifier3"")))//继电器三
        {
            judge_data_sta(""identifier3"", GPIO_Pin_11); //判断电平状态并发布给app
            LED3 = !identifier_data;//控制继电器
        }
        else if(!(strcmp(identifier_temp, ""identifier4"")))//继电器四
        {
            judge_data_sta(""identifier4"", GPIO_Pin_12); //判断电平状态并发布给app
            LED4 = !identifier_data;//控制继电器
        }
        else if(!(strcmp(identifier_temp, ""identifier5"")))//窗帘
        {
            if(identifier_data)
            {
                data_sta(""identifier5"", 1);
                MotorRun(2048, 0, 5000);
            }
            else
            {
                data_sta(""identifier5"", 0);
                MotorRun(2048, 1, 5000);
            }
        }
        else if(!(strcmp(identifier_temp, ""identifier6"")))//自动模式
        {
            auto_flag = identifier_data;//调整模式
        }
        else if(!(strcmp(identifier_temp, ""identifier7"")))//电视
        {
            if(identifier_data)
            {
                IR_ctrl_on[3] = 0x02;
                for(int i = 0; i < 5; i++)
                {
                    send_byte(USART3, IR_ctrl_on[i]);
                    send_byte(USART1, IR_ctrl_on[i]);
                }
            }
            else
            {
                IR_ctrl_off[3] = 0x03;
                for(int i = 0; i < 5; i++)
                {
                    send_byte(USART3, IR_ctrl_off[i]);
                    send_byte(USART1, IR_ctrl_off[i]);
                }
            }
        }
        else if(!(strcmp(identifier_temp, ""identifier8"")))//电视
        {
            if(identifier_data)
            {
                IR_ctrl_on[3] = 0x04;
                for(int i = 0; i < 5; i++)
                {
                    send_byte(USART3, IR_ctrl_on[i]);
                    send_byte(USART1, IR_ctrl_on[i]);
                }
            }
            else
            {
                IR_ctrl_off[3] = 0x05;
                for(int i = 0; i < 5; i++)
                {
                    send_byte(USART3, IR_ctrl_off[i]);
                    send_byte(USART1, IR_ctrl_off[i]);
                }
            }
        }
        else if(!(strcmp(identifier_temp, ""identifier9"")))//电视
        {
            if(identifier_data)
            {
                IR_ctrl_on[3] = 0x06;
                for(int i = 0; i < 5; i++)
                {
                    send_byte(USART3, IR_ctrl_on[i]);
                    send_byte(USART1, IR_ctrl_on[i]);
                }
            }
            else
            {
                IR_ctrl_off[3] = 0x07;
                for(int i = 0; i < 5; i++)
                {
                    send_byte(USART3, IR_ctrl_off[i]);
                    send_byte(USART1, IR_ctrl_off[i]);
                }
            }
        }
        else if(!(strcmp(identifier_temp, ""identifier12"")))//电视
        {
            if(identifier_data)
            {
                IR_ctrl_on[3] = 0x00;
                for(int i = 0; i < 5; i++)
                {
                    send_byte(USART3, IR_ctrl_on[i]);
                    send_byte(USART1, IR_ctrl_on[i]);
                }
            }
            else
            {
                IR_ctrl_off[3] = 0x01;
                for(int i = 0; i < 5; i++)
                {
                    send_byte(USART3, IR_ctrl_off[i]);
                    send_byte(USART1, IR_ctrl_off[i]);
                }
            }
        }
        else if(!(strcmp(identifier_temp, ""identifier15""))) //烟雾上限
            PPM_MAX = identifier_data; //将数据赋值
        else if(!(strcmp(identifier_temp, ""identifier16""))) //一氧化碳上限
            co_MAX = identifier_data; //将数据赋值
        memset(identifier_temp, 0, 50);
    }
    MQTT_CMDOutPtr += CBUFF_UNIT;                             	 //指针下移
    if(MQTT_CMDOutPtr == MQTT_CMDEndPtr)           	           //如果指针到缓冲区尾部了
        MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	             //指针归位到缓冲区开头
}
/*-------------------------------------------------*/
/*函数名：定时上传各种数据的任务                   */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
extern unsigned int SystemTimer ;
extern unsigned int TEHUTimer ;
void Data_State(void)
{
    char temp[500];             //缓冲区
    if((SystemTimer - TEHUTimer) >= 6)                                    //全局时间 和 计时时间至少差1s，进入if
    {
        TEHUTimer = SystemTimer;                                           //把当前的全局时间，记录到温湿度计时变量
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\""identifier18"\":%d,\""identifier19"\":%d,\""identifier17"\":%d,\""identifier13"\":%d,\""identifier14"\":%d,\""identifier20"\":%d},\"version\":\"1.0.0\"}", tem, hum, light, co, ppm, people); //构建数据
        MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                //添加数据到发送缓冲区
    }
}
void init_State(void)
{

    char temp[600];             //缓冲区
    sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\""identifier18"\":%d,\""identifier19"\":%d,\""identifier17"\":%d,\""identifier13"\":%d,\""identifier14"\":%d,\""identifier15"\":%d,\
	\""identifier16"\":%d,\""identifier1"\":0,\""identifier2"\":0,\""identifier3"\":0,\""identifier4"\":0},\"version\":\"1.0.0\"}", tem, hum, light, co, ppm, PPM_MAX, co_MAX); //构建数据
    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                //添加数据到发送缓冲区

}



	/*-------------------------------------------------*/
	/*函数名：发布开关的状态给服务器                   */
	/*参  数：无                                       */
	/*返回值：无                                       */
	/*-------------------------------------------------*/
	void data_sta(void *str, int cmd)
	{
			char temp[TBUFF_UNIT];                   //定义一个临时缓冲区
			memset(temp, 0, TBUFF_UNIT);
			sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"%s\":%d},\"version\":\"1.0.0\"}", str, cmd); //如果LED2是高电平，说明是熄灭状态，需要回复关闭状态给服务器
			MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp)); //添加数据，发布给服务器
	}
	/*-------------------------------------------------*/
	/*函数名：发布判断开关的状态给服务器                   */
	/*参  数：无                                       */
	/*返回值：无                                       */
	/*-------------------------------------------------*/
	void judge_data_sta(void *str, uint16_t GPIO_Pin)
	{
			char temp[TBUFF_UNIT];                   //定义一个临时缓冲区
			memset(temp, 0, TBUFF_UNIT);
			if( GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin))//判断io的电平
					sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"%s\":1},\"version\":\"1.0.0\"}", str); //如果LED2是高电平，说明是熄灭状态，需要回复关闭状态给服务器
			else
					sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"%s\":0},\"version\":\"1.0.0\"}", str); //如果LED2是高电平，说明是熄灭状态，需要回复关闭状态给服务器
			MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp)); //添加数据，发布给服务器
	}


