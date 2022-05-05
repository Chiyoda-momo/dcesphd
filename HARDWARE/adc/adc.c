#include<adc.h>

void adc1_config(){
	GPIO_InitTypeDef GPIO_InitStruceD;//结构体属于变量，变量的声明必须位于函数可执行的语句之前
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);//开启GPIOD的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//开启ADC1的时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //设置分频因子为6.
	
	GPIO_InitStruceD.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_InitStruceD.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOD,&GPIO_InitStruceD);	
	
	ADC_DeInit(ADC1);//复位ADC1
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                //ADC工作模式:ADC1和ADC2工作在独立模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       //模数转换工作在单通道模式
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                 //模数转换工作在单次转换模式
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //转换由软件而不是外部触发启动
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              //ADC数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel = 1;                             //顺序进行规则转换的ADC通道的数目
  ADC_Init(ADC1,&ADC_InitStructure);                                  //初始化ADC1
    
  ADC_Cmd(ADC1,ENABLE);                        //使能ADC
	
  ADC_ResetCalibration(ADC1);                  //使能复位校准
  while(ADC_GetResetCalibrationStatus(ADC1));  //等待复位校准结束
  ADC_StartCalibration(ADC1);                  //开启AD校准
  while(ADC_GetCalibrationStatus(ADC1));       //等待校准结束
  ADC_SoftwareStartConvCmd(ADC1,ENABLE);       //使能ADC1的软件转换启动功能
}

void adc2_config(){
	GPIO_InitTypeDef GPIO_InitStruceD;//结构体属于变量，变量的声明必须位于函数可执行的语句之前
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);//开启GPIOD的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);//开启ADC2的时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //设置分频因子为6.
	
	GPIO_InitStruceD.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_InitStruceD.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOD,&GPIO_InitStruceD);	
	
	ADC_DeInit(ADC2);//复位ADC2
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                //ADC工作模式:ADC1和ADC2工作在独立模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       //模数转换工作在单通道模式
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                 //模数转换工作在单次转换模式
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //转换由软件而不是外部触发启动
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              //ADC数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel = 1;                             //顺序进行规则转换的ADC通道的数目
  ADC_Init(ADC2,&ADC_InitStructure);                                  //初始化ADC2
    
  ADC_Cmd(ADC2,ENABLE);                        //使能ADC
	
  ADC_ResetCalibration(ADC2);                  //使能复位校准
  while(ADC_GetResetCalibrationStatus(ADC2));  //等待复位校准结束
  ADC_StartCalibration(ADC2);                  //开启AD校准
  while(ADC_GetCalibrationStatus(ADC2));       //等待校准结束
  ADC_SoftwareStartConvCmd(ADC2,ENABLE);       //使能ADC2的软件转换启动功能
}


u16 get_adc1(){
  //配置ADCx，ADC通道，规则采样顺序，采样时间
  ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);
  ADC_SoftwareStartConvCmd(ADC1,ENABLE);          //使能ADC1的软件转换启动功能
  while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));   //等待转换结束
  return ADC_GetConversionValue(ADC1);            //返回最近一次ADC1规则组的转换结果
}

u16 get_adc2(){
	//配置ADCx，ADC通道，规则采样顺序，采样时间
  ADC_RegularChannelConfig(ADC2,ADC_Channel_10,1,ADC_SampleTime_239Cycles5);
  ADC_SoftwareStartConvCmd(ADC2,ENABLE);          //使能ADC1的软件转换启动功能
  while(!ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC));   //等待转换结束
  return ADC_GetConversionValue(ADC2);            //返回最近一次ADC2规则组的转换结果
}

