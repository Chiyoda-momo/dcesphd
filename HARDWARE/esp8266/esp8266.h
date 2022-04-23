#ifndef __ESP8266_H
#define __ESP8266_H
#include "sys.h"
#include "stdio.h"
//�û�����ATָ��
extern char restart[];
extern char cwmode[];
extern char cwlap[];
extern char cwjap[];
extern char cifsr[];
extern char cipmux[];
extern char cipstart[];
extern char cipsend[];
extern char cipserver[];
extern char cwlif[];
extern char cipstatus[];
extern char cipsto[];
extern char cipmode[];
extern char test[];

//����esp8266.
//����
//const char rst[] = "AT+RST\r\n";
//AT+CWMODE=1 ����STAģʽ,��Ϊ�ͻ���
//const char mode[] = "AT+CWMODE=1\r\n";
//AT+CWLAP ��ѯ����WIFI
//AT+CWJAP="Yuzu","35623788MyDesk"
//const char wifi[] = "AT+CWJAP=\"Yuzu\",\"35623788MyDesk\"\r\n";
//AT+CIFSR ��ѯ����ĵ�ַ
//AT+CIPMUX=1 �򿪶�����
//const char mux[] = "AT+CIPMUX=1\r\n";
//AT+CIPMUX=0 �򿪶�����
//const char unmux[] = "AT+CIPMUX=0\r\n";
//AT+CIPSERVER=1,8088 ����ģ��������˿�
//const char serverport[] = "AT+CIPSERVER=1,8088\r\n";
//͸��ģʽ����
//const char cipmodopen[] = "AT+CIPMODE=<1>\r\n";
//͸��ģʽ�ر�
//const char cipmodclose[] = "+++\r";
//����
void esp8266_Config(void);
u8 esp8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8* esp8266_send_data(u8 *cmd,u16 waittime);
u8* esp8266_check_cmd(u8 *str);
void esp8266_start_trans(void);
u8 esp8266_quit_trans(void);	
#endif

