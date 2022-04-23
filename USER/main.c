#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usart1.h"
#include "esp8266.h"
#include "string.h"
#include "timer7.h"
 int main(void)
 {	
	u8 t;
	u8 len;	
	u16 times=0; 
	 
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
	 
	 delay_init();
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 uart_init(9600);
	 usart3_init(9600);
	 printf("开始esp8266初始化!");
	 esp8266_Config();
	 
	 while(1)
	{
	    GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	    GPIO_SetBits(GPIOD,GPIO_Pin_2);
			Delay(3000000);
			GPIO_SetBits(GPIOA,GPIO_Pin_8);
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);
			Delay(3000000);
			printf("esp8266传输数据开始!");
			esp8266_start_trans();
			delay_ms(100);
			esp8266_send_data("12",50);
			delay_ms(100);
			esp8266_quit_trans();
	}
 }

