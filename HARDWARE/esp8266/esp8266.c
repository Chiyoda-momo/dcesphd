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
	printf("��ʼ���ù���ģʽ!");
	//���ù���ģʽ 1��stationģʽ   2��APģʽ  3������ AP+stationģʽ
	esp8266_send_cmd("AT+CWMODE=1","OK",500);
	printf("���ù���ģʽΪstationģʽ���!");
	//��Wifiģ������������
	printf("��������!");
	esp8266_send_cmd("AT+RST","[Vendor:www.ai-thinker.com Version:0.9.2.4]\rready",500);

	delay_ms(1000);         //��ʱ3S�ȴ������ɹ�
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	printf("�������");
	//��ģ���������Լ���·��
	//while(esp8266_send_cmd("AET0","OK",500));
	while(esp8266_send_cmd("AT+CWJAP=\"YuzuDeskTop\",\"6C606666\"","OK",600));
	printf("wifi������!");
	//=0����·����ģʽ     =1����·����ģʽ
	esp8266_send_cmd("AT+CIPMUX=0","OK",500);
	printf("�����õ�·����!");
	//����TCP����  ������ֱ������ Ҫ���ӵ�ID��0~4   ��������  Զ�̷�����IP��ַ   Զ�̷������˿ں�
	while(esp8266_send_cmd("AT+CIPSTART=\"TCP\",\"192.168.137.1\",10005","OK",600));
	printf("tcp�����ѽ���!");
	
	
}

//ESP8266ģ���PC����͸��ģʽ
void esp8266_start_trans(void)
{
	//�Ƿ���͸��ģʽ  0����ʾ�ر� 1����ʾ����͸��
	while(esp8266_send_cmd("AT+CIPMODE=1","OK",500));
	//͸��ģʽ�� ��ʼ�������ݵ�ָ�� ���ָ��֮��Ϳ���ֱ�ӷ�������
	esp8266_send_cmd("AT+CIPSEND","OK",600);
	printf("͸��ģʽ���������ѿ�ʼ!");
}

//ESP8266�˳�͸��ģʽ   ����ֵ:0,�˳��ɹ�;1,�˳�ʧ��
//����wifiģ�飬ͨ����wifiģ����������3��+��ÿ��+��֮�� ����10ms,������Ϊ���������η���+��
u8 esp8266_quit_trans(void)
{
	u8 result=1;
	u3_printf("%s","+++");
	Delay(300000000);			//�ȴ�500ms̫�� Ҫ1000ms�ſ����˳�
	result=esp8266_send_cmd("AT","OK",500);//�˳�͸���ж�.
	if(result){
		u3_printf("%s","+++");
		Delay(300000000);
		result=esp8266_send_cmd("AT","OK",500);//�˳�͸���ж�.
		if(result){
			printf("�˳�͸��ģʽʧ��!!");
		}else{
			printf("�˳�͸��ģʽ�ɹ�!!");
			while(esp8266_send_cmd("AT+CIPMODE=0","OK",500));
		}
	}else{
		printf("�˳�͸��ģʽ�ɹ�!");
		while(esp8266_send_cmd("AT+CIPMODE=0","OK",500));
	}
	return result;
}


//��ESP8266��������
//cmd:���͵������ַ���;ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��;waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����);1,����ʧ��
u8 esp8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	printf("��ʼ��������!");
	u3_printf("%s\r\n",cmd);	//��������
	printf("�����������!�ȴ�Ӧ��");
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(esp8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					//printf("��ȡ�������!");
					break;//�õ���Ч���� 
				}
					USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 


//ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����;����,�ڴ�Ӧ������λ��(str��λ��)
u8* esp8266_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
		printf("���յ�����Ϊ:\n");
		uart_send_buff((u8*)strx,strlen(strx));
		USART3_RX_STA = 0;
	} 
	return (u8*)strx;
}


//��ESP8266��������
//cmd:���͵������ַ���;waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:�������ݺ󣬷������ķ�����֤��
u8* esp8266_send_data(u8 *cmd,u16 waittime)
{
	char temp[5];
	char *ack=temp;
	USART3_RX_STA=0;
	u3_printf("%s",cmd);	//��������
	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
				ack=(char*)USART3_RX_BUF;
				//printf("�������ݺ�Ľ���:ack:%s\r\n",(u8*)ack);
				USART3_RX_STA=0;
				break;//�õ���Ч���� 
			} 
		}
	}
	return (u8*)ack;
} 
