#ifndef _DATAPROCESSING_H
#define _DATAPROCESSING_H

//���ݴ���,��Ҫ����ESP8266��������Ϣ,��������Щ��Ϣ���ж˿ڵĿ���
#include<stm32f10x_gpio.h>

extern u8* esp8266_data;

void get_data(u8* data);//��ȡ����

void data_process(void);//��������

void gpio_output_control(char gpio_group,u8 gpio_pin,u8 gpio_status);//��������˿ڿ���

void gpio_input_data_process(char gpio_group,u8 gpio_pin,u8 process_type);//���������ݽ��д���

#endif
