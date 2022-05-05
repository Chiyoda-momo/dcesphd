#ifndef _GPIO_H
#define _GPIO_H
//GPIO配置,配置需要使用的GPIO
//STM32F103RCT6可用IO如下:
/*
	GPIOA_Pin_0 ~ GPIOA_Pin_15
	GPIOB_Pin_0 ~ GPIOB_Pin_15
	GPIOC_Pin_0 ~ GPIOC_Pin_15
	GPIOD_Pin_0 ~ GPIOD_Pin_2
*/
//一共51个可用IO.其中,PA9,PA10,PB10,PB11已经用于USART.故现在只有47个可用IO.

#include<stm32f10x_gpio.h>

void gpio_config(void);//总配置

void gpio_control_output_config(void);//用作输出端的gpio配置

void gpio_control_input_config(void);//用作输入端的gpio配置


#endif
