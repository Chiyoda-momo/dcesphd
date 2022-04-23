#ifndef __ESP8266_H
#define __ESP8266_H
#include "sys.h"
#include "stdio.h"
//用户配置AT指令
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

//配置esp8266.
//重启
//const char rst[] = "AT+RST\r\n";
//AT+CWMODE=1 设置STA模式,作为客户端
//const char mode[] = "AT+CWMODE=1\r\n";
//AT+CWLAP 查询附近WIFI
//AT+CWJAP="Yuzu","35623788MyDesk"
//const char wifi[] = "AT+CWJAP=\"Yuzu\",\"35623788MyDesk\"\r\n";
//AT+CIFSR 查询分配的地址
//AT+CIPMUX=1 打开多连接
//const char mux[] = "AT+CIPMUX=1\r\n";
//AT+CIPMUX=0 打开多连接
//const char unmux[] = "AT+CIPMUX=0\r\n";
//AT+CIPSERVER=1,8088 设置模块服务器端口
//const char serverport[] = "AT+CIPSERVER=1,8088\r\n";
//透传模式开启
//const char cipmodopen[] = "AT+CIPMODE=<1>\r\n";
//透传模式关闭
//const char cipmodclose[] = "+++\r";
//配置
void esp8266_Config(void);
u8 esp8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8* esp8266_send_data(u8 *cmd,u16 waittime);
u8* esp8266_check_cmd(u8 *str);
void esp8266_start_trans(void);
u8 esp8266_quit_trans(void);	
#endif

