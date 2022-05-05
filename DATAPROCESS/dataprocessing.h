#ifndef _DATAPROCESSING_H
#define _DATAPROCESSING_H

//数据处理,主要处理ESP8266发来的消息,并根据这些信息进行端口的控制
#include<stm32f10x_gpio.h>

extern u8* esp8266_data;

void get_data(u8* data);//获取数据

void data_process(void);//处理数据

void gpio_output_control(char gpio_group,u8 gpio_pin,u8 gpio_status);//进行输出端口控制

void gpio_input_data_process(char gpio_group,u8 gpio_pin,u8 process_type);//对输入数据进行处理

#endif
