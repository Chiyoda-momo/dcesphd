#include<time6.h>
#include<adc.h>
u16 time=0;
 
//通用定时器6中断初始化，这里时钟选择为APB1的2倍
//arr：自动重装值 psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz 
//通用定时器中断初始化 
//2000 * 72  / 72000000即为2ms
void TIM6_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);//TIM6时钟使能    
	
	//定时器TIM6初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //使能指定的TIM6中断,允许更新中断
	
	TIM_Cmd(TIM6,ENABLE);//开启定时器6
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 4;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}

//设置为2ms取样一次
void TIM6_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)//是更新中断
	{	 			   
		if(time > 499){
			time=0;
		}else{
			u16 adc1Data = get_adc1();
			u16 adc2Data = get_adc2();
			adc1_data[time] = adc1Data;
			adc2_data[time] = adc2Data;
			time++;
		}
	}    
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //清除TIM6更新中断标志  
}
