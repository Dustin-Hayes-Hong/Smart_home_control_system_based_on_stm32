#include "sys.h"
//turn 函数将整数型转换为字符串
//m为要转换的整数型 
//str为保存的数组
//n为欲转换的个数
//t标志位 标志是否余留空位
void turn(int m,char str[],int n)//n=265
{
	int i=0,j=1,k=0;
	int t=1;//是否空位标志位
	char c;
	if(m<0)//记录数据
	{	str[i++]='-';
		m=-m;
	}	
	do
	{	str[i]='0'+m%10;
		m=m/10;
		i++;
	}while(m>0);

	//处理符号
	if(str[0]=='-')j=1;
	else j=0;
	k=i-1;
	while(j<k)
	{
		c=str[j];
		str[j]=str[k];
		str[k]=c;
		j++;
		k--;
	}
	if(t)
	{
		while(i-1<=n)//此循环将剩余位置弄成空格	
		{
			str[i]=' ';
			i++;
		}
	}
	str[i]='\0';
//	return (i-1);
}




void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
