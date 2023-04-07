
#ifndef __TOTAL_WIFI_H
#define __TOTAL_WIFI_H
#include "stm32f10x.h"  //包含需要的头文件
#include "main.h"       //包含需要的头文件
#include "delay.h"      //包含需要的头文件
#include "usart1.h"     //包含需要的头文件
#include "usart2.h"     //包含需要的头文件
#include "timer3_ping.h"     //包含需要的头文件
#include "pub_sub_message.h"
#include "led.h"        //包含需要的头文件
#include "key.h"        //包含需要的头文件
#include "string.h"       //包含需要的头文件
#include "stdio.h"        //包含需要的头文件
#include "utils_hmac.h"   //包含需要的头文件


void Relay_State(void);
void Relay1_State(void);
void LED_State(void);
void LED1_State(void);
void LED2_State(void);

void Relay3_State(void);
void Relay4_State(void);
void Relay2_State(void);

void Connect_server(void);
void  publish_message(void);
extern unsigned int SystemTimer,SystemTimer2,SystemTimer3,SystemTimer4,SystemTimer5,SystemTimer6;   //用于全局计时的变量           单位秒
extern unsigned int TEHUTimer,TEHUTimer2,TEHUTimer3,TEHUTimer4,TEHUTimer5,TEHUTimer6 ;     //用于温湿度采集的计时的变量   单位秒


/*------------------------------------------------------------ mqtt的头文件----------------------------------------------------*/
/*------------------------------------------------------------ mqtt的头文件----------------------------------------------------*/
/*------------------------------------------------------------ mqtt的头文件----------------------------------------------------*/
#define  R_NUM               8     //接收缓冲区个数
#define  RBUFF_UNIT          300   //接收缓冲区长度

#define  T_NUM               8     //发送缓冲区个数  
#define  TBUFF_UNIT          300   //发送缓冲区长度

#define  C_NUM               8     //命令缓冲区个数
#define  CBUFF_UNIT          300   //命令缓冲区长度

#define  MQTT_TxData(x)       u2_TxData(x)                                         //串口2负责数据发送
#define SSID   "A"                     //初始化的路由器SSID名称
#define PASS   "1234567890."                 //初始化的路由器密码






#define  PRODUCTKEY           "a1guKy2Gvxw"                                        //产品ID
#define  PRODUCTKEY_LEN       strlen(PRODUCTKEY)                                   //产品ID长度


#define  DEVICENAME           "ABCD"                                               //设备名  
#define  DEVICENAME_LEN       strlen(DEVICENAME)                                   //设备名长度

#define  DEVICESECRE          "32ec72eb87349ba5a9d6d00a18340347"                   //设备秘钥   
#define  DEVICESECRE_LEN      strlen(DEVICESECRE)                                  //设备秘钥长度

#define  S_TOPIC_NAME         "/sys/a1guKy2Gvxw/ABCD/thing/service/property/set"   //需要订阅的主题 

#define  P_TOPIC_NAME         "/sys/a1guKy2Gvxw/ABCD/thing/event/property/post"    //需要发布的主题  

extern unsigned char  MQTT_RxDataBuf[R_NUM][RBUFF_UNIT];       //外部变量声明，数据的接收缓冲区,所有服务器发来的数据，存放在该缓冲区,缓冲区第一个字节存放数据长度
extern unsigned char *MQTT_RxDataInPtr;                        //外部变量声明，指向缓冲区存放数据的位置
extern unsigned char *MQTT_RxDataOutPtr;                       //外部变量声明，指向缓冲区读取数据的位置
extern unsigned char *MQTT_RxDataEndPtr;                       //外部变量声明，指向缓冲区结束的位置
extern unsigned char  MQTT_TxDataBuf[T_NUM][TBUFF_UNIT];       //外部变量声明，数据的发送缓冲区,所有发往服务器的数据，存放在该缓冲区,缓冲区第一个字节存放数据长度
extern unsigned char *MQTT_TxDataInPtr;                        //外部变量声明，指向缓冲区存放数据的位置
extern unsigned char *MQTT_TxDataOutPtr;                       //外部变量声明，指向缓冲区读取数据的位置
extern unsigned char *MQTT_TxDataEndPtr;                       //外部变量声明，指向缓冲区结束的位置
extern unsigned char  MQTT_CMDBuf[C_NUM][CBUFF_UNIT];          //外部变量声明，命令数据的接收缓冲区
extern unsigned char *MQTT_CMDInPtr;                           //外部变量声明，指向缓冲区存放数据的位置
extern unsigned char *MQTT_CMDOutPtr;                          //外部变量声明，指向缓冲区读取数据的位置
extern unsigned char *MQTT_CMDEndPtr;                          //外部变量声明，指向缓冲区结束的位置

extern char ClientID[128];     //外部变量声明，存放客户端ID的缓冲区
extern int  ClientID_len;      //外部变量声明，存放客户端ID的长度
extern char Username[128];     //外部变量声明，存放用户名的缓冲区
extern int  Username_len;	   //外部变量声明，存放用户名的长度
extern char Passward[128];     //外部变量声明，存放密码的缓冲区
extern int  Passward_len;	   //外部变量声明，存放密码的长度
extern char ServerIP[128];     //外部变量声明，存放服务器IP或是域名
extern int  ServerPort;        //外部变量声明，存放服务器的端口号

extern char Ping_flag;         //外部变量声明，ping报文状态      0：正常状态，等待计时时间到，发送Ping报文
                               //外部变量声明，ping报文状态      1：Ping报文已发送，当收到 服务器回复报文的后 将1置为0
extern char Connect_flag;      //外部变量声明，同服务器连接状态  0：还没有连接服务器  1：连接上服务器了
extern char ReConnect_flag;    //外部变量声明，重连服务器状态    0：连接还存在  1：连接断开，重连
extern char ConnectPack_flag;  //外部变量声明，CONNECT报文状态   1：CONNECT报文成功
extern char SubcribePack_flag; //外部变量声明，订阅报文状态      1：订阅报文成功

void MQTT_Buff_Init(void);
void AliIoT_Parameter_Init(void);
void MQTT_ConectPack(void);
void MQTT_Subscribe(char *, int);
void MQTT_PingREQ(void);
void MQTT_PublishQs0(char *, char *, int);
void MQTT_DealPushdata_Qs0(unsigned char *);	
void TxDataBuf_Deal(unsigned char *, int);
void CMDBuf_Deal(unsigned char *, int);
void Data_State(void);
/*------------------------------------------------------------ mqtt的头文件----------------------------------------------------*/
/*------------------------------------------------------------ mqtt的头文件----------------------------------------------------*/
/*------------------------------------------------------------ mqtt的头文件----------------------------------------------------*/


/*---------------------------wifi的头文件--------------------------------------------------------*/
/*---------------------------wifi的头文件--------------------------------------------------------*/
/*---------------------------wifi的头文件--------------------------------------------------------*/
#define RESET_IO(x)    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)x)  //PA4控制WiFi的复位

#define WiFi_printf       u2_printf           //串口2控制 WiFi
#define WiFi_RxCounter    Usart2_RxCounter    //串口2控制 WiFi
#define WiFi_RX_BUF       Usart2_RxBuff       //串口2控制 WiFi
#define WiFi_RXBUFF_SIZE  USART2_RXBUFF_SIZE  //串口2控制 WiFi



void WiFi_ResetIO_Init(void);
char WiFi_SendCmd(char *cmd, int timeout);
char WiFi_Reset(int timeout);
char WiFi_JoinAP(int timeout);
char WiFi_Connect_Server(int timeout);
char WiFi_Smartconfig(int timeout);
char WiFi_WaitAP(int timeout);
char WiFi_Connect_IoTServer(void);
extern void data_caiji(void);
/*---------------------------wifi的头文件--------------------------------------------------------*/
/*---------------------------wifi的头文件--------------------------------------------------------*/
/*---------------------------wifi的头文件--------------------------------------------------------*/
#endif
