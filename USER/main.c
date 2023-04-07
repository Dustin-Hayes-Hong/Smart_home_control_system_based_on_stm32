#include "total_wifi.h"
#include "pub_sub_message.h"
#include "DMA_adc.h"
#include "show.h"
#include "time4.h"
#include "usart3.h"
#include "infrared.h"
#include "dht11.h"
#include "motorrun.h"        //包含需要的头文件
void data_caiji(void);
void ctrl_auto(void);
extern char SubcribePack_flag;   //订阅报文状态      1：订阅报文成功
unsigned int SystemTimer = 0;   //用于全局计时的变量           单位秒
unsigned int TEHUTimer = 0;     //用于温湿度采集的计时的变量   单位秒
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];

int Learn_IR_ctrl[5] = {0xFA, 0xFD, 0x01, 0x00, 0xDF};//发送学习遥控的指令

int IR_ctrl_on[5] = {0xFA, 0xFD, 0x02, 0x00, 0xDF};//发送控制的指令
int IR_ctrl_off[5] = {0xFA, 0xFD, 0x02, 0x01, 0xDF };//发送控制的指令

u8 light, ppm, co;//传感器的数据
u16 adc_light, adc_ppm, adc_co;//采集到的ADC的值
u8 tem, hum, people;//传感器的数据
u8 PPM_MAX = 50, co_MAX = 50;//数据的上限
int main(void)// A1 F1 01 FA DE         A1 F1 02 F6 FD    A1 F1 01 59 DE
{
    Delay_Init();                   //延时功能初始化
    Usart1_Init(9600);              //串口1功能初始化，波特率9600
    uart3_init(9600);
 
    //LED初始化
    KEY_Init();                     //按键初始化
    KEY_Exti_Init();//外部中断初始化
    ADCx_Init();//adc采集初始
    OLED_Init();//oled显示初始化
    WiFi_ResetIO_Init();            //初始化WiFi的复位IO
    AliIoT_Parameter_Init();	    //初始化连接阿里云IoT平台MQTT服务器的参数
    LED_Init();
    MOTOR_Init();//步进电机初始化
    DHT11_Init();//温湿度初始
    TIM2_ENABLE_200MS();//定时上传的定时器初始化
    while(1)                      //主循环
    { 
        Connect_server();//连接服务器的函数
        if(SubcribePack_flag)//订阅完主题后才进来
        {
						data_caiji();//采集数据函数
            ctrl_auto();//自动控制的函数
            Cut_screen();//切屏函数       
            voice_ctrl();//语音控制
        }
    }
}
u8 people_num_sta,people_num;//用于判断人数是否增加的
void data_caiji(void)
{
    if(IO_A5_IN_STA)//判断是否高电平 高电平则有人
    {
			printf("1");
			people_num=1;		
			if(people_num!=people_num_sta)
			{
				people_num_sta=people_num;
				people++;//检测到的人数++
			}
    }
		else 
		{	printf("0");
			people_num_sta=0;//将状态清零 便于下次使用
		}
    adc_light = Get_Adc_Average(0, 10); //ADC湿度获取
    if(adc_light > 4000)adc_light = 4000;
    light = (100 - (adc_light / 40));

    /***************烟雾采集***********************/
    adc_ppm = Get_Adc_Average(1, 10); //读取通道7的AD值，20ms读取一次    2335   2730
    ppm = adc_ppm * 99 / 4096; //12位AD，把AD值转换成百分比0~99
    ppm = ppm >= 99 ? 99 : ppm; //最大值不能超过99
    /***************烟雾采集***********************/
    /***************烟雾采集***********************/
    adc_co = Get_Adc_Average(2, 10); //读取通道7的AD值，20ms读取一次    2335   2730
    co = adc_co * 99 / 4096; //12位AD，把AD值转换成百分比0~99
    co = co >= 99 ? 99 : co; //最大值不能超过99
    /***************烟雾采集***********************/
    DHT11_Read_Data(&hum, &tem);
}
u8 auto_flag ;//自动标志位
u8 ppm_beep, ppm_beep_sta;//报警的标志位 和报警标志位上一次的状态
u8 co_beep, co_beep_sta;//报警的标志位 和报警标志位上一次的状态
void ctrl_auto(void)
{
    char temp[500];
    if(auto_flag)//自动模式置位
    {
        if(ppm > PPM_MAX)//判断是否大于上限
        {
            BEEP = !BEEP;//蜂鸣器报警
            ppm_beep = 1;//报警标志位置位  用于上传app报警
						Delay_Ms(30);
            if(ppm_beep != ppm_beep_sta)//判断是否需要上传报警
            {
                show_num = 4;//跳转显示报警信息的屏幕
                ppm_beep_sta = ppm_beep;//将报警标志记录 防止一直进来上传报警
                if(SubcribePack_flag)//订阅完主题后才进来
                {
                    sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\""identifier10"\":1},\"version\":\"1.0.0\"}");
                    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp)); //添加数据，发布给服务器
                }
            }
        }
        else if(ppm < PPM_MAX)
        {
            BEEP = 1;//蜂鸣器取消报警
            ppm_beep = 0;//报警标志位取消置位  用于上传app报警
            if(ppm_beep != ppm_beep_sta)//判断是否需要上传报警
            {
                show_num = 0;//跳转显示主的屏幕
                ppm_beep_sta = ppm_beep;//将报警标志记录 防止一直进来上传报警
                if(SubcribePack_flag)//订阅完主题后才进来
                {
                    sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\""identifier10"\":0},\"version\":\"1.0.0\"}");
                    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp)); //添加数据，发布给服务器
                }
            }
        }
        if(co > co_MAX)
        {
            BEEP = !BEEP;
						Delay_Ms(30);
            co_beep = 1;
            show_num = 4;//跳转显示报警信息的屏幕
            if(co_beep != co_beep_sta)//判断是否需要上传报警
            {
                co_beep_sta = co_beep;//将报警标志记录 防止一直进来上传报警
                if(SubcribePack_flag)//订阅完主题后才进来
                {
                    sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\""identifier11"\":1},\"version\":\"1.0.0\"}");
                    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp)); //添加数据，发布给服务器
                }
            }
        }
        else if(co < co_MAX)
        {
            BEEP = 1;
            co_beep = 0;
            if(co_beep != co_beep_sta)//判断是否需要上传报警
            {
                show_num = 0;//跳转显示主的屏幕
                co_beep_sta = co_beep;
                if(SubcribePack_flag)//订阅完主题后才进来
                {
                    sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\""identifier11"\":0},\"version\":\"1.0.0\"}");
                    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp)); //添加数据，发布给服务器
                }
            }
        }
    }

}





