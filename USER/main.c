#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usart1.h"
#include "esp8266.h"
#include "string.h"
#include "timer7.h"
#include "time6.h"
#include "gpio.h"
#include "adc.h"
#include "dataprocessing.h"

u16 adc1_data[500];//adc1的数据
u16 adc2_data[500];//adc2的数据
u16 sendAdcData[500];//要传给服务端的adc数据
u8 sendGpiocData;//要传给服务端的检测端数据
u8 sendAllData[630];//最终封装成要传给服务端的数据.
u8 processOK;//解析是否正常标志位

 int main(void)
 {	
	u16 t;
	u16 len;	
	 
  GPIO_InitTypeDef  GPIO_InitStructure;
 	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能PD端口时钟
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PD.2 端口配置, 推挽输出
  GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
  GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 //PD.2 输出高 	  

	 gpio_config();//io配置
	 delay_init();//延时配置
	 adc1_config();
	 adc2_config();
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断配置
	 uart_init(115200);//串口1配置
	 usart3_init(115200);//串口3配置
	 TIM6_Int_Init(2000 - 1,72 - 1);//定时器6配置,2ms进行一次模拟输入取样 
	 printf("开始esp8266初始化!");
	 esp8266_Config();//ESP8266配置
	 
	 while(1)
	{
	    GPIO_SetBits(GPIOD,GPIO_Pin_2);
			delay_ms(1000);
			delay_ms(1000);
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);
			delay_ms(1000);
			delay_ms(1000);
		
			//测试端消息
		/*if(USART_RX_STA&0x8000){
			//如果串口1有消息
				len = USART_RX_STA&0x3fff;	
				u8 data[len];
				printf("发送的消息是:\n");
				for(t = 0;t < len;t++){
					USART1->DR=USART_RX_BUF[t];
					data[t] = USART_RX_BUF[t];
					while((USART1->SR&0X40)==0);
				}
				set_data(data);
				data_process();//数据处理
				if(!processOK){
					printf("解析出错!!!\n");
				}
				printf("把数据传给服务端\n");
				USART_RX_STA = 0;
				printf("esp8266传输数据开始!");
				esp8266_start_trans();

				delay_ms(100);
				printf("这是串口1发送给单片机的内容:");
				for(u8 i = 0;i < 10;i++){
					esp8266_send_data(data,50);
				}
				delay_ms(100);
				esp8266_quit_trans();
			}
			*/
			//ESP8266透传消息
			if(USART3_RX_STA&0X8000){
				//esp8266收到消息
				len = USART3_RX_STA&0x3fff;
				u8 data[len];
				for(t = 0;t < len;t++){
					data[t] = USART3_RX_BUF[t];
				}
				USART3_RX_STA = 0;
				//需要进行数据处理,data[len]就是服务器发送的数据
				//需要的数据是以{开头,}结尾的.
				u16 datalen = 0;
				u8 js = 0;
				u16 start = 0;
				u16 end = 0;
				for(t = 0;t < len;t++){
					if(data[t] == '{'){
						js = 1;
						start = t;
					}else if(data[t] == '}'){
						js = 0;
						datalen++;
						end = t;
					}
					if(js){
						datalen++;
					}
				}
				u8 trueData[datalen];
				js = 0;
				for(;start <= end;start++){
						trueData[js++] = data[start];
				}
				set_data(trueData);
				data_process();//数据处理
	
				if(!processOK){
					printf("解析出错!!!\n");
					delay_ms(1000);
					delay_ms(1000);
					delay_ms(1000);
					esp8266_start_trans();
					for(u8 i = 0;i < 5;i++){
						esp8266_send_data("解析数据出错.",500);
						delay_ms(500);
					}
					for(u8 i = 0;i < 5;i++){
						esp8266_send_data("callfinish",500);
						delay_ms(500);
					}
					esp8266_quit_trans();
				}else{
					//数据打包和回传
					//要打包的数据:sendAdcData,sendGpiocData.打包到sendAllData中.
					char* str1 = "adc:[";
					char* str2 = "check:";
					u16 index = 0;
					while(*str1 != '\0'){
						sendAllData[index++] = *str1;
						str1++;
					}
					for(u16 i = 0;i < 300;i++){
						sendAllData[index++] = ((sendAdcData[i] >> 8) & 0xff) + 48;//高八位
						sendAllData[index++] = (sendAdcData[i] & 0xff) + 48;//低八位
					}
					sendAllData[index++] = ']';
					sendAllData[index++] = ',';
					while(*str2 != '\0'){
						sendAllData[index++] = *str2;
						str2++;
					}
					char gpiocData[8];
					sprintf(gpiocData,"%X",sendGpiocData);
					for(u8 i = 0;i < 8;i++){
						sendAllData[index++] = gpiocData[i];
					}
					//发送给服务器
					delay_ms(1000);
					delay_ms(1000);
					esp8266_start_trans();
					for(u8 i = 0;i < 5;i++){
						esp8266_send_data(sendAllData,50);
						delay_ms(10);
					}
					
					delay_ms(100);
					for(u8 i = 0;i < 5;i++){
						esp8266_send_data("callfinish",500);//结束标志
						delay_ms(500);
					}
					delay_ms(1000);
					esp8266_quit_trans();
				}
				
			}
			printf("运行中\n");
			delay_ms(1000);
	}
 }

