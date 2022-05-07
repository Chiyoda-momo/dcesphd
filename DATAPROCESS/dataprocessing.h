#ifndef _DATAPROCESSING_H
#define _DATAPROCESSING_H

//���ݴ���,��Ҫ����ESP8266��������Ϣ,��������Щ��Ϣ���ж˿ڵĿ���
#include<stm32f10x_gpio.h>
#include<cJSON.h>
#include<time6.h>
/**
�ⶨ: A0~A2Ϊ��Դ���ƶ�,����3-8������,���ư�·�̵���ͨ�Ͽ���оƬ��Դ.
��Ҫ16���˿���Ϊ���ƶ˿�,8�����Ƽ̵���,8�������ź�����.
A3~A8,A11-A12��Ϊ8·����̵����Ŀ��ƶ˿�
A13-A15,B0-B4��Ϊ8·�̵����������źŶ˿�.
C0-C7Ϊ�ź�����˿�.
*/


extern u16 sendAdcData[500];
extern u8 sendGpiocData;
extern u8 processOK; // �����Ƿ�������־λ

void set_data(u8* data);//��ȡ����

void data_process(void);//��������

void gpio_output_control(char gpio_group,u8 gpio_pin,u8 gpio_status,u8 isBinaryData);//��������˿ڿ���

void gpio_input_data_process(u8 gpio_pin,u8 process_type);//���������ݽ��д���

void gpio_a3a8_a11a12_binaryDataSet(u8 binaryData);

void gpio_a13a15_b0b4_binaryDataSet(u8 binaryData);

void gpioa_control(u8 pin,u8 gpio_status);

void gpiob_control(u8 pin,u8 gpio_status);

u8 get_gpioc_data(u8 pin);

u8 get_gpioc08_data(void);

u8 get_gpioc09_data(void);

u16* get_adc(u8 adcPort);

#endif
