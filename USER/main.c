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

u16 adc1_data[500];//adc1������
u16 adc2_data[500];//adc2������
u16 sendAdcData[500];//Ҫ��������˵�adc����
u8 sendGpiocData;//Ҫ��������˵ļ�������
u8 sendAllData[630];//���շ�װ��Ҫ��������˵�����.
u8 processOK;//�����Ƿ�������־λ

 int main(void)
 {	
	u16 t;
	u16 len;	
	 
  GPIO_InitTypeDef  GPIO_InitStructure;
 	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PD�˿�ʱ��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PD.2 �˿�����, �������
  GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
  GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 //PD.2 ����� 	  

	 gpio_config();//io����
	 delay_init();//��ʱ����
	 adc1_config();
	 adc2_config();
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�ж�����
	 uart_init(115200);//����1����
	 usart3_init(115200);//����3����
	 TIM6_Int_Init(2000 - 1,72 - 1);//��ʱ��6����,2ms����һ��ģ������ȡ�� 
	 printf("��ʼesp8266��ʼ��!");
	 esp8266_Config();//ESP8266����
	 
	 while(1)
	{
	    GPIO_SetBits(GPIOD,GPIO_Pin_2);
			delay_ms(1000);
			delay_ms(1000);
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);
			delay_ms(1000);
			delay_ms(1000);
		
			//���Զ���Ϣ
		/*if(USART_RX_STA&0x8000){
			//�������1����Ϣ
				len = USART_RX_STA&0x3fff;	
				u8 data[len];
				printf("���͵���Ϣ��:\n");
				for(t = 0;t < len;t++){
					USART1->DR=USART_RX_BUF[t];
					data[t] = USART_RX_BUF[t];
					while((USART1->SR&0X40)==0);
				}
				set_data(data);
				data_process();//���ݴ���
				if(!processOK){
					printf("��������!!!\n");
				}
				printf("�����ݴ��������\n");
				USART_RX_STA = 0;
				printf("esp8266�������ݿ�ʼ!");
				esp8266_start_trans();

				delay_ms(100);
				printf("���Ǵ���1���͸���Ƭ��������:");
				for(u8 i = 0;i < 10;i++){
					esp8266_send_data(data,50);
				}
				delay_ms(100);
				esp8266_quit_trans();
			}
			*/
			//ESP8266͸����Ϣ
			if(USART3_RX_STA&0X8000){
				//esp8266�յ���Ϣ
				len = USART3_RX_STA&0x3fff;
				u8 data[len];
				for(t = 0;t < len;t++){
					data[t] = USART3_RX_BUF[t];
				}
				USART3_RX_STA = 0;
				//��Ҫ�������ݴ���,data[len]���Ƿ��������͵�����
				//��Ҫ����������{��ͷ,}��β��.
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
				data_process();//���ݴ���
	
				if(!processOK){
					printf("��������!!!\n");
					delay_ms(1000);
					delay_ms(1000);
					delay_ms(1000);
					esp8266_start_trans();
					for(u8 i = 0;i < 5;i++){
						esp8266_send_data("�������ݳ���.",500);
						delay_ms(500);
					}
					for(u8 i = 0;i < 5;i++){
						esp8266_send_data("callfinish",500);
						delay_ms(500);
					}
					esp8266_quit_trans();
				}else{
					//���ݴ���ͻش�
					//Ҫ���������:sendAdcData,sendGpiocData.�����sendAllData��.
					char* str1 = "adc:[";
					char* str2 = "check:";
					u16 index = 0;
					while(*str1 != '\0'){
						sendAllData[index++] = *str1;
						str1++;
					}
					for(u16 i = 0;i < 300;i++){
						sendAllData[index++] = ((sendAdcData[i] >> 8) & 0xff) + 48;//�߰�λ
						sendAllData[index++] = (sendAdcData[i] & 0xff) + 48;//�Ͱ�λ
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
					//���͸�������
					delay_ms(1000);
					delay_ms(1000);
					esp8266_start_trans();
					for(u8 i = 0;i < 5;i++){
						esp8266_send_data(sendAllData,50);
						delay_ms(10);
					}
					
					delay_ms(100);
					for(u8 i = 0;i < 5;i++){
						esp8266_send_data("callfinish",500);//������־
						delay_ms(500);
					}
					delay_ms(1000);
					esp8266_quit_trans();
				}
				
			}
			printf("������\n");
			delay_ms(1000);
	}
 }

