#ifndef _DATAPROCESSING_H
#define _DATAPROCESSING_H

//数据处理,主要处理ESP8266发来的消息,并根据这些信息进行端口的控制
#include<stm32f10x_gpio.h>
#include<cJSON.h>
#include<time6.h>
/**
拟定: A0~A2为电源控制端,接入3-8译码器,控制八路继电器通断开启芯片电源.
需要16个端口作为控制端口,8个控制继电器,8个进行信号输入.
A3~A8,A11-A12作为8路输入继电器的控制端口
A13-A15,B0-B4作为8路继电器的输入信号端口.
C0-C7为信号输入端口.
*/


extern u16 sendAdcData[500];
extern u8 sendGpiocData;
extern u8 processOK; // 解析是否正常标志位

void set_data(u8* data);//获取数据

void data_process(void);//处理数据

void gpio_output_control(char gpio_group,u8 gpio_pin,u8 gpio_status,u8 isBinaryData);//进行输出端口控制

void gpio_input_data_process(u8 gpio_pin,u8 process_type);//对输入数据进行处理

void gpio_a3a8_a11a12_binaryDataSet(u8 binaryData);

void gpio_a13a15_b0b4_binaryDataSet(u8 binaryData);

void gpioa_control(u8 pin,u8 gpio_status);

void gpiob_control(u8 pin,u8 gpio_status);

u8 get_gpioc_data(u8 pin);

u8 get_gpioc08_data(void);

u8 get_gpioc09_data(void);

u16* get_adc(u8 adcPort);

#endif
