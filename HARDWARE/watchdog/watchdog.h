//独立看门狗,每两秒钟检查一次8266的返回,如果返回ERROR,说明连接寄了,需要重启
#ifndef _WATCHDOG_
#define _WATCHDOG_


#include<usart1.h>
#include<stm32f10x_iwdg.h>

void watch_dog_init(u8 prer,u16 rlr);

void feed_dog(void);



#endif
