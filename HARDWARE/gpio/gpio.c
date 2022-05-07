#include <gpio.h>

void gpio_config(){
	gpio_control_output_config();
	gpio_control_input_config();
}
//28������˿�
void gpio_control_output_config(){
	//GPIOA_Config---------------------------------------------------------------------------------------------------------
	GPIO_InitTypeDef GPIO_InitStruceA;//�ṹ�����ڱ�������������������λ�ں�����ִ�е����֮ǰ
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//����GPIOA��ʱ��
	
	GPIO_InitStruceA.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_InitStruceA.GPIO_Pin = GPIO_Pin_0 || GPIO_Pin_1 || GPIO_Pin_2 || GPIO_Pin_3 || GPIO_Pin_4 || GPIO_Pin_5 
														|| GPIO_Pin_6 || GPIO_Pin_7 || GPIO_Pin_11 || GPIO_Pin_12 || GPIO_Pin_13
														|| GPIO_Pin_14 || GPIO_Pin_15;//PA0~PA7,PA11~PA15
	
	GPIO_InitStruceA.GPIO_Speed = GPIO_Speed_50MHz;//����Ϊ����ģʽʱ���������·��˿��ǶϿ�����������ٶ����������塣

	GPIO_Init(GPIOA,&GPIO_InitStruceA);
	//GPIOA_Config---------------------------------------------------------------------------------------------------------
	//GPIOB_Config---------------------------------------------------------------------------------------------------------
	GPIO_InitTypeDef GPIO_InitStruceB;//�ṹ�����ڱ�������������������λ�ں�����ִ�е����֮ǰ
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//����GPIOB��ʱ��
	
	GPIO_InitStruceB.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_InitStruceB.GPIO_Pin = GPIO_Pin_0 || GPIO_Pin_1 || GPIO_Pin_2 || GPIO_Pin_3 || GPIO_Pin_4 || GPIO_Pin_5 
														|| GPIO_Pin_6 || GPIO_Pin_7 || GPIO_Pin_8 || GPIO_Pin_9 || GPIO_Pin_12 || GPIO_Pin_13
														|| GPIO_Pin_14 || GPIO_Pin_15;//PB0~PA9,PA12~PA15
	
	GPIO_InitStruceB.GPIO_Speed = GPIO_Speed_50MHz;//����Ϊ����ģʽʱ���������·��˿��ǶϿ�����������ٶ����������塣

	GPIO_Init(GPIOB,&GPIO_InitStruceB);
	//GPIOB_Config---------------------------------------------------------------------------------------------------------
}
//16������˿�,2��ģ������˿�,ģ������˿�������adc��
void gpio_control_input_config(){
	//GPIOC_Config---------------------------------------------------------------------------------------------------------
	GPIO_InitTypeDef GPIO_InitStruceC;//�ṹ�����ڱ�������������������λ�ں�����ִ�е����֮ǰ
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//����GPIOB��ʱ��
	
	GPIO_InitStruceC.GPIO_Mode = GPIO_Mode_IPU;//��������
	GPIO_InitStruceC.GPIO_Pin = GPIO_Pin_0 || GPIO_Pin_1 || GPIO_Pin_2 || GPIO_Pin_3 || GPIO_Pin_4 || GPIO_Pin_5 
														|| GPIO_Pin_6 || GPIO_Pin_7 || GPIO_Pin_8 || GPIO_Pin_9 || GPIO_Pin_10 || GPIO_Pin_11
														|| GPIO_Pin_12 || GPIO_Pin_13 || GPIO_Pin_14 || GPIO_Pin_15;//PC0~PC15
	
	//GPIO_InitStruceC.GPIO_Speed = GPIO_Speed_50MHz;//����Ϊ����ģʽʱ���������·��˿��ǶϿ�����������ٶ����������塣

	GPIO_Init(GPIOC,&GPIO_InitStruceC);	
	//GPIOC_Config---------------------------------------------------------------------------------------------------------
}
