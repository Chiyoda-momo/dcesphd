//�������Ź�,ÿ�����Ӽ��һ��8266�ķ���,�������ERROR,˵�����Ӽ���,��Ҫ����
#ifndef _WATCHDOG_
#define _WATCHDOG_


#include<usart1.h>
#include<stm32f10x_iwdg.h>

void watch_dog_init(u8 prer,u16 rlr);

void feed_dog(void);



#endif
