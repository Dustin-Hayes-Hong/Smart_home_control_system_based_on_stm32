#include "infrared.h"
uint8_t buf[128], cnt1;
uint16_t bufLen;
void key_IR()
{
	usart1_echo();//串口一回显
		switch (IR) 
		{
			case 1:	//长按2秒进入红外学习
					bufLen = IR_Learn_Pack(buf, 0x01);//将数据组合并返回长度
					send_str_len(USART3,buf,bufLen);//发送指令给红外
			
		break;
		case 2:	//单击或者双击红外发射
		case 3:
						bufLen = IR_Send_Pack(buf, 0x01);//将数据组合并返回长度
						send_str_len(USART3,buf,bufLen);//发送指令给红外
		break;
		default:  IR=0;
		break;
	
	}
}
void usart1_echo(void)//串口一回显
{
		if(Usart3_Receive)//串口四接收完成
		{	
			Usart3_Receive=0;//清除标志位
			send_str_len(USART1,Usart3_RxBuff,Usart3_RxCompleted);	//将红外模块发送回来的数据显示在pc
			Usart3_RxCompleted = 0;//接收数组下标清理	
 			memset(Usart3_RxBuff,0,sizeof(Usart3_RxBuff));//清理接收数组
		}
}
//求校验和
static uint8_t Get_Check(uint8_t *data, uint16_t len) {
	uint8_t sum = 0;
	uint16_t i;
	for (i = 0; i < len; i++) {
		sum += data[i];
	}
	return sum;
}

//红外内码学习指令组包
uint16_t IR_Learn_Pack(uint8_t *data, uint8_t index) {
	uint8_t *p = data;
	*p++ = FRAME_START;
	*p++ = 0x08;
	*p++ = 0x00;
	*p++ = MODULE_ADDR;
	*p++ = 0x10;
	*p++ = 0x01;

	*p = Get_Check(&data[3], p - data - 3);
	 p++;
	*p++ = FRAME_END;
	return p - data;
}

//红外内码发送指令组包
uint16_t IR_Send_Pack(uint8_t *data, uint8_t index) {
	uint8_t *p = data;
	*p++ = FRAME_START;
	*p++ = 0x08;
	*p++ = 0x00;
	*p++ = MODULE_ADDR;
	*p++ = 0x12;
	*p++ = index;
	*p = Get_Check(&data[3], p - data - 3);
	p++;
	*p++ = FRAME_END;
	return p - data;
}
