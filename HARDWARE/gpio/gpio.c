#include <gpio.h>

void gpio_config(){
	gpio_control_output_config();
	gpio_control_input_config();
}
//28个输出端口
void gpio_control_output_config(){
	//GPIOA_Config---------------------------------------------------------------------------------------------------------
	GPIO_InitTypeDef GPIO_InitStruceA;//结构体属于变量，变量的声明必须位于函数可执行的语句之前
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//开启GPIOA的时钟
	
	GPIO_InitStruceA.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStruceA.GPIO_Pin = GPIO_Pin_0 || GPIO_Pin_1 || GPIO_Pin_2 || GPIO_Pin_3 || GPIO_Pin_4 || GPIO_Pin_5 
														|| GPIO_Pin_6 || GPIO_Pin_7 || GPIO_Pin_11 || GPIO_Pin_12 || GPIO_Pin_13
														|| GPIO_Pin_14 || GPIO_Pin_15;//PA0~PA7,PA11~PA15
	
	GPIO_InitStruceA.GPIO_Speed = GPIO_Speed_50MHz;//配置为输入模式时输出驱动电路与端口是断开，所以输出速度配置无意义。

	GPIO_Init(GPIOA,&GPIO_InitStruceA);
	//GPIOA_Config---------------------------------------------------------------------------------------------------------
	//GPIOB_Config---------------------------------------------------------------------------------------------------------
	GPIO_InitTypeDef GPIO_InitStruceB;//结构体属于变量，变量的声明必须位于函数可执行的语句之前
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//开启GPIOB的时钟
	
	GPIO_InitStruceB.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStruceB.GPIO_Pin = GPIO_Pin_0 || GPIO_Pin_1 || GPIO_Pin_2 || GPIO_Pin_3 || GPIO_Pin_4 || GPIO_Pin_5 
														|| GPIO_Pin_6 || GPIO_Pin_7 || GPIO_Pin_8 || GPIO_Pin_9 || GPIO_Pin_12 || GPIO_Pin_13
														|| GPIO_Pin_14 || GPIO_Pin_15;//PB0~PA9,PA12~PA15
	
	GPIO_InitStruceB.GPIO_Speed = GPIO_Speed_50MHz;//配置为输入模式时输出驱动电路与端口是断开，所以输出速度配置无意义。

	GPIO_Init(GPIOB,&GPIO_InitStruceB);
	//GPIOB_Config---------------------------------------------------------------------------------------------------------
}
//16个输入端口,2个模拟输入端口,模拟输入端口配置在adc中
void gpio_control_input_config(){
	//GPIOC_Config---------------------------------------------------------------------------------------------------------
	GPIO_InitTypeDef GPIO_InitStruceC;//结构体属于变量，变量的声明必须位于函数可执行的语句之前
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//开启GPIOB的时钟
	
	GPIO_InitStruceC.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStruceC.GPIO_Pin = GPIO_Pin_0 || GPIO_Pin_1 || GPIO_Pin_2 || GPIO_Pin_3 || GPIO_Pin_4 || GPIO_Pin_5 
														|| GPIO_Pin_6 || GPIO_Pin_7 || GPIO_Pin_8 || GPIO_Pin_9 || GPIO_Pin_10 || GPIO_Pin_11
														|| GPIO_Pin_12 || GPIO_Pin_13 || GPIO_Pin_14 || GPIO_Pin_15;//PC0~PC15
	
	//GPIO_InitStruceC.GPIO_Speed = GPIO_Speed_50MHz;//配置为输入模式时输出驱动电路与端口是断开，所以输出速度配置无意义。

	GPIO_Init(GPIOC,&GPIO_InitStruceC);	
	//GPIOC_Config---------------------------------------------------------------------------------------------------------
}
