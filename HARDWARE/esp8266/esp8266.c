#include "esp8266.h"
#include "usart.h"
#include "esp8266.h"
#include "string.h"
#include "usart1.h"
#include "stm32f10x.h"
#include "sys.h" 
#include "delay.h"
void Delay(u32 count)
 {
  u32 i=0;
  for(;i<count;i++);

 }
void esp8266_Config(void){
	//while(esp8266_quit_trans());
	delay_ms(500);
	printf("开始设置工作模式!");
	//设置工作模式 1：station模式   2：AP模式  3：兼容 AP+station模式
	esp8266_send_cmd("AT+CWMODE=1","OK",500);
	printf("设置工作模式为station模式完成!");
	//让Wifi模块重启的命令
	printf("进行重启!");
	esp8266_send_cmd("AT+RST","[Vendor:www.ai-thinker.com Version:0.9.2.4]\rready",500);

	delay_ms(1000);         //延时3S等待重启成功
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	printf("重启完成");
	//让模块连接上自己的路由
	//while(esp8266_send_cmd("AET0","OK",500));
	while(esp8266_send_cmd("AT+CWJAP=\"YuzuDeskTop\",\"6C606666\"","OK",600));
	printf("wifi已连接!");
	//=0：单路连接模式     =1：多路连接模式
	esp8266_send_cmd("AT+CIPMUX=0","OK",500);
	printf("已设置单路连接!");
	//建立TCP连接  这四项分别代表了 要连接的ID号0~4   连接类型  远程服务器IP地址   远程服务器端口号
	while(esp8266_send_cmd("AT+CIPSTART=\"TCP\",\"192.168.137.1\",10005","OK",600));
	printf("tcp连接已建立!");
	
	
}

//ESP8266模块和PC进入透传模式
void esp8266_start_trans(void)
{
	//是否开启透传模式  0：表示关闭 1：表示开启透传
	while(esp8266_send_cmd("AT+CIPMODE=1","OK",500));
	//透传模式下 开始发送数据的指令 这个指令之后就可以直接发数据了
	esp8266_send_cmd("AT+CIPSEND","OK",600);
	printf("透传模式传输数据已开始!");
}

//ESP8266退出透传模式   返回值:0,退出成功;1,退出失败
//配置wifi模块，通过想wifi模块连续发送3个+（每个+号之间 超过10ms,这样认为是连续三次发送+）
u8 esp8266_quit_trans(void)
{
	u8 result=1;
	u3_printf("%s","+++");
	Delay(300000000);			//等待500ms太少 要1000ms才可以退出
	result=esp8266_send_cmd("AT","OK",500);//退出透传判断.
	if(result){
		u3_printf("%s","+++");
		Delay(300000000);
		result=esp8266_send_cmd("AT","OK",500);//退出透传判断.
		if(result){
			printf("退出透传模式失败!!");
		}else{
			printf("退出透传模式成功!!");
			while(esp8266_send_cmd("AT+CIPMODE=0","OK",500));
		}
	}else{
		printf("退出透传模式成功!");
		while(esp8266_send_cmd("AT+CIPMODE=0","OK",500));
	}
	return result;
}


//向ESP8266发送命令
//cmd:发送的命令字符串;ack:期待的应答结果,如果为空,则表示不需要等待应答;waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果);1,发送失败
u8 esp8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	printf("开始发送命令!");
	u3_printf("%s\r\n",cmd);	//发送命令
	printf("发送命令完成!等待应答");
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(esp8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					//printf("获取数据完成!");
					break;//得到有效数据 
				}
					USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 


//ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果;其他,期待应答结果的位置(str的位置)
u8* esp8266_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
		printf("接收的数据为:\n");
		uart_send_buff((u8*)strx,strlen(strx));
		USART3_RX_STA = 0;
	} 
	return (u8*)strx;
}


//向ESP8266发送数据
//cmd:发送的命令字符串;waittime:等待时间(单位:10ms)
//返回值:发送数据后，服务器的返回验证码
u8* esp8266_send_data(u8 *cmd,u16 waittime)
{
	char temp[5];
	char *ack=temp;
	USART3_RX_STA=0;
	u3_printf("%s",cmd);	//发送命令
	if(waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
				ack=(char*)USART3_RX_BUF;
				//printf("发送数据后的接收:ack:%s\r\n",(u8*)ack);
				USART3_RX_STA=0;
				break;//得到有效数据 
			} 
		}
	}
	return (u8*)ack;
} 
