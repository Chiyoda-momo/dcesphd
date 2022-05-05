#include<adc.h>

void adc1_config(){
	GPIO_InitTypeDef GPIO_InitStruceD;//�ṹ�����ڱ�������������������λ�ں�����ִ�е����֮ǰ
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);//����GPIOD��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//����ADC1��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //���÷�Ƶ����Ϊ6.
	
	GPIO_InitStruceD.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	GPIO_InitStruceD.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOD,&GPIO_InitStruceD);	
	
	ADC_DeInit(ADC1);//��λADC1
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                //ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       //ģ��ת�������ڵ�ͨ��ģʽ
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                 //ģ��ת�������ڵ���ת��ģʽ
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //ת��������������ⲿ��������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              //ADC�����Ҷ���
  ADC_InitStructure.ADC_NbrOfChannel = 1;                             //˳����й���ת����ADCͨ������Ŀ
  ADC_Init(ADC1,&ADC_InitStructure);                                  //��ʼ��ADC1
    
  ADC_Cmd(ADC1,ENABLE);                        //ʹ��ADC
	
  ADC_ResetCalibration(ADC1);                  //ʹ�ܸ�λУ׼
  while(ADC_GetResetCalibrationStatus(ADC1));  //�ȴ���λУ׼����
  ADC_StartCalibration(ADC1);                  //����ADУ׼
  while(ADC_GetCalibrationStatus(ADC1));       //�ȴ�У׼����
  ADC_SoftwareStartConvCmd(ADC1,ENABLE);       //ʹ��ADC1�����ת����������
}

void adc2_config(){
	GPIO_InitTypeDef GPIO_InitStruceD;//�ṹ�����ڱ�������������������λ�ں�����ִ�е����֮ǰ
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);//����GPIOD��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);//����ADC2��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //���÷�Ƶ����Ϊ6.
	
	GPIO_InitStruceD.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	GPIO_InitStruceD.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOD,&GPIO_InitStruceD);	
	
	ADC_DeInit(ADC2);//��λADC2
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                //ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       //ģ��ת�������ڵ�ͨ��ģʽ
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                 //ģ��ת�������ڵ���ת��ģʽ
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //ת��������������ⲿ��������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              //ADC�����Ҷ���
  ADC_InitStructure.ADC_NbrOfChannel = 1;                             //˳����й���ת����ADCͨ������Ŀ
  ADC_Init(ADC2,&ADC_InitStructure);                                  //��ʼ��ADC2
    
  ADC_Cmd(ADC2,ENABLE);                        //ʹ��ADC
	
  ADC_ResetCalibration(ADC2);                  //ʹ�ܸ�λУ׼
  while(ADC_GetResetCalibrationStatus(ADC2));  //�ȴ���λУ׼����
  ADC_StartCalibration(ADC2);                  //����ADУ׼
  while(ADC_GetCalibrationStatus(ADC2));       //�ȴ�У׼����
  ADC_SoftwareStartConvCmd(ADC2,ENABLE);       //ʹ��ADC2�����ת����������
}


u16 get_adc1(){
  //����ADCx��ADCͨ�����������˳�򣬲���ʱ��
  ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);
  ADC_SoftwareStartConvCmd(ADC1,ENABLE);          //ʹ��ADC1�����ת����������
  while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));   //�ȴ�ת������
  return ADC_GetConversionValue(ADC1);            //�������һ��ADC1�������ת�����
}

u16 get_adc2(){
	//����ADCx��ADCͨ�����������˳�򣬲���ʱ��
  ADC_RegularChannelConfig(ADC2,ADC_Channel_10,1,ADC_SampleTime_239Cycles5);
  ADC_SoftwareStartConvCmd(ADC2,ENABLE);          //ʹ��ADC1�����ת����������
  while(!ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC));   //�ȴ�ת������
  return ADC_GetConversionValue(ADC2);            //�������һ��ADC2�������ת�����
}

