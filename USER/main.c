#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usart1.h"
#include "esp8266.h"
#include "string.h"
#include "timer7.h"
#include "gpio.h"
#include "adc.h"
#include "dataprocessing.h"

 int main(void)
 {	
	u8 t,istc;
	u8 len;	
	 
  GPIO_InitTypeDef  GPIO_InitStructure;
 	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);	 //使能PA,PD端口时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PA.8 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
  GPIO_SetBits(GPIOA,GPIO_Pin_8);						 //PA.8 输出高

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PD.2 端口配置, 推挽输出
  GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
  GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 //PD.2 输出高 	  

	 gpio_config();//io配置
	 delay_init();//延时配置
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断配置
	 uart_init(9600);//串口1配置
	 usart3_init(9600);//串口3配置
	 printf("开始esp8266初始化!");
	 esp8266_Config();//ESP8266配置
	 istc = 1;
	 
	 while(1)
	{
	    GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	    GPIO_SetBits(GPIOD,GPIO_Pin_2);
			Delay(3000000);
			GPIO_SetBits(GPIOA,GPIO_Pin_8);
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);
			Delay(3000000);
			if(USART_RX_STA&0x8000){
			//如果串口1有东西,也就是来自测试端的消息
				len = USART_RX_STA&0x3fff;	
				u8 data[len];
				printf("发送的消息是:\n");
				for(t = 0;t < len;t++){
					USART1->DR=USART_RX_BUF[t];
					data[t] = USART_RX_BUF[t];
					while((USART1->SR&0X40)==0);
				}
				printf("把数据传给服务端\n");
				USART_RX_STA = 0;
				printf("esp8266传输数据开始!");
				if(istc == 0){
					esp8266_start_trans();
					printf("透传模式已启动");
					istc = 1;
				}
				delay_ms(100);
				printf("这是串口1发送给单片机的内容:");
				esp8266_send_data(data,50);
				delay_ms(100);
				esp8266_quit_trans();
				istc = 0;
			}
			if(USART3_RX_STA&0X8000){
				//esp8266收到消息
				len = USART3_RX_STA&0x3fff;
				printf("接收到服务器的数据为:\n");
				for(t = 0;t < len;t++){
					USART1->DR=USART3_RX_BUF[t];
					while((USART1->SR&0X40)==0);
				}
				//需要进行数据处理
				
				//数据打包和回传
				
				//发送给服务器
				Delay(10000000);
				printf("服务器你的消息是:");
				if(istc == 0){
					esp8266_start_trans();
					printf("透传模式已启动");
					istc = 1;
				}
				delay_ms(100);
				esp8266_send_data(USART3_RX_BUF,500);
				delay_ms(100);
				esp8266_quit_trans();
				istc = 0;
				USART3_RX_STA = 0;
			}
			Delay(10000000);
	}
 }

