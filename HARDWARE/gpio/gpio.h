#ifndef _GPIO_H
#define _GPIO_H
//GPIO����,������Ҫʹ�õ�GPIO
//STM32F103RCT6����IO����:
/*
	GPIOA_Pin_0 ~ GPIOA_Pin_15
	GPIOB_Pin_0 ~ GPIOB_Pin_15
	GPIOC_Pin_0 ~ GPIOC_Pin_15
	GPIOD_Pin_0 ~ GPIOD_Pin_2
*/
//һ��51������IO.����,PA9,PA10,PB10,PB11�Ѿ�����USART.������ֻ��47������IO.

#include<stm32f10x_gpio.h>

void gpio_config(void);//������

void gpio_control_output_config(void);//��������˵�gpio����

void gpio_control_input_config(void);//��������˵�gpio����


#endif
