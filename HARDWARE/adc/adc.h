//配置ADC数模转换,以处理来自GPIOD_Pin0,GPIOD_Pin1的ADC信号
#ifndef _ADC_H
#define _ADC_H
#include <stm32f10x_adc.h>

void adc1_config(void);

void adc2_config(void);

u16 get_adc1(void);

u16 get_adc2(void);

#endif
