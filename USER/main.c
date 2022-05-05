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
 	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PA,PD�˿�ʱ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PA.8 �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
  GPIO_SetBits(GPIOA,GPIO_Pin_8);						 //PA.8 �����

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PD.2 �˿�����, �������
  GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
  GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 //PD.2 ����� 	  

	 gpio_config();//io����
	 delay_init();//��ʱ����
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�ж�����
	 uart_init(9600);//����1����
	 usart3_init(9600);//����3����
	 printf("��ʼesp8266��ʼ��!");
	 esp8266_Config();//ESP8266����
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
			//�������1�ж���,Ҳ�������Բ��Զ˵���Ϣ
				len = USART_RX_STA&0x3fff;	
				u8 data[len];
				printf("���͵���Ϣ��:\n");
				for(t = 0;t < len;t++){
					USART1->DR=USART_RX_BUF[t];
					data[t] = USART_RX_BUF[t];
					while((USART1->SR&0X40)==0);
				}
				printf("�����ݴ��������\n");
				USART_RX_STA = 0;
				printf("esp8266�������ݿ�ʼ!");
				if(istc == 0){
					esp8266_start_trans();
					printf("͸��ģʽ������");
					istc = 1;
				}
				delay_ms(100);
				printf("���Ǵ���1���͸���Ƭ��������:");
				esp8266_send_data(data,50);
				delay_ms(100);
				esp8266_quit_trans();
				istc = 0;
			}
			if(USART3_RX_STA&0X8000){
				//esp8266�յ���Ϣ
				len = USART3_RX_STA&0x3fff;
				printf("���յ�������������Ϊ:\n");
				for(t = 0;t < len;t++){
					USART1->DR=USART3_RX_BUF[t];
					while((USART1->SR&0X40)==0);
				}
				//��Ҫ�������ݴ���
				
				//���ݴ���ͻش�
				
				//���͸�������
				Delay(10000000);
				printf("�����������Ϣ��:");
				if(istc == 0){
					esp8266_start_trans();
					printf("͸��ģʽ������");
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

