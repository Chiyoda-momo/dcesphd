//进行数据处理和端口控制
#include<dataprocessing.h>
#include<stdio.h>
#include<delay.h>

u8* esp8266_data = NULL; // 从服务端获取的JSON字符串
u8 haveData = 0; // 是否有数据标志位

void set_data(u8* data){
	esp8266_data = data;
	processOK = 1;
	haveData = 1;
}

/**
esp8266_data格式: 
{lowerPowerTo : ["c5",...],powerToNode : ["p2",...],highPowerTo : ["c4",...],nodeToCheck : ["c1",...]}
为一个JSON字符串格式,故使用cJSON进行解析
*/
void data_processing(){
	if(!esp8266_data){
		//如果数据是空的,就结束
		haveData = 0;
		processOK = 0;//解析出问题
		return;
	}
	//不为空就做处理
	cJSON* data = cJSON_Parse((char*)esp8266_data);//根据获取的数据构造cJSON对象
	if(!data){
		//获取的JSON字符串有问题!
		processOK = 0;//解析出问题
		return;
	}
	//四个都是cJSON数组对象.
	cJSON* lowerPowerTo = cJSON_GetObjectItem(data,"lowerPowerTo");
	cJSON* powerToNode = cJSON_GetObjectItem(data,"powerToNode");
	cJSON* highPowerTo = cJSON_GetObjectItem(data,"highPowerTo");
	cJSON* nodeToCheck = cJSON_GetObjectItem(data,"nodeToCheck");
	//分别解析
	//这四个都是数组类型
	//首先解析powerToNode
	//当前powerToNode只允许有一个存在,直接按常量比较
	if(powerToNode){
		for(int i = 0;i < cJSON_GetArraySize(powerToNode);i++){
			cJSON* power = cJSON_GetArrayItem(powerToNode,i);
			if(cJSON_String == power->type){
				//获取端口
				char* groupAndPin = power->valuestring;
				//char group = groupAndPin[0];//P,表示控制
        u8 pin = 0;//引脚
        u8 numBit = 0;
        while(*groupAndPin != '\0'){
           if(*groupAndPin >= '0' && *groupAndPin <= '9'){
               numBit++;
					 }
           groupAndPin++;
        }
        groupAndPin = power->valuestring;
        if(numBit == 2){
           pin += (groupAndPin[1] - '0') * 10;
           pin += (groupAndPin[2] - '0');
        }else{
           pin += (groupAndPin[1] - '0');
        }
				//直接处理
				switch(pin){
					case 0:
						//A0~A2 : 000
						gpio_output_control('A',0,0,0);
						gpio_output_control('A',1,0,0);
						gpio_output_control('A',2,0,0);
					break;
					case 1:
						//A0~A2 : 001
						gpio_output_control('A',0,0,0);
						gpio_output_control('A',1,0,0);
						gpio_output_control('A',2,1,0);
					break;
					case 2:
						//A0~A2 : 010
						gpio_output_control('A',0,0,0);
						gpio_output_control('A',1,1,0);
						gpio_output_control('A',2,0,0);
					break;
					case 3:
						//A0~A2 : 011
						gpio_output_control('A',0,0,0);
						gpio_output_control('A',1,1,0);
						gpio_output_control('A',2,1,0);
					break;
					case 4:
						//A0~A2 : 100
						gpio_output_control('A',0,1,0);
						gpio_output_control('A',1,0,0);
						gpio_output_control('A',2,0,0);
					break;
					case 5:
						//A0~A2 : 101
						gpio_output_control('A',0,1,0);
						gpio_output_control('A',1,0,0);
						gpio_output_control('A',2,1,0);
					break;
					case 6:
						//A0~A2 : 110
						gpio_output_control('A',0,1,0);
						gpio_output_control('A',1,1,0);
						gpio_output_control('A',2,0,0);
					break;
					case 7:
						//A0~A2 : 111
						gpio_output_control('A',0,1,0);
						gpio_output_control('A',1,1,0);
						gpio_output_control('A',2,1,0);
					break;
				}
			}
		}
	}
	delay_ms(1000);
	//解析lowerPowerTo和highPowerTo
	u8 lowerAndHighPowerControl = 0;
	u8 lowerAndHighPowerBit = 0;
	if(lowerPowerTo){
		u8 controlBit = 0;
		for(int i = 0;i < cJSON_GetArraySize(lowerPowerTo);i++){
			cJSON* lowerPower = cJSON_GetArrayItem(lowerPowerTo,i);
			if(cJSON_String == lowerPower->type){
				//获取端口
				char* groupAndPin = lowerPower->valuestring;
				//char group = groupAndPin[0];//C,表示控制
        u8 pin = 0;//引脚
        u8 numBit = 0;
        while(*groupAndPin != '\0'){
           if(*groupAndPin >= '0' && *groupAndPin <= '9'){
               numBit++;
					 }
           groupAndPin++;
        }
        groupAndPin = lowerPower->valuestring;
        if(numBit == 2){
           pin += (groupAndPin[1] - '0') * 10;
           pin += (groupAndPin[2] - '0');
        }else{
           pin += (groupAndPin[1] - '0');
        }
				controlBit |= pin;
			}
		}
		lowerAndHighPowerControl |= controlBit;
		lowerAndHighPowerBit |= ~controlBit;
		//直接处理
		//A3~A8,A11~A12 : 0000_0000 继电器控制
		//A13~A15,B0~B4 : 0000_0000 电平输入
	}
	if(highPowerTo){
		u8 controlBit = 0;
		for(int i = 0;i < cJSON_GetArraySize(highPowerTo);i++){
			cJSON* highPower = cJSON_GetArrayItem(highPowerTo,i);
			if(cJSON_String == highPower->type){
				//获取端口
				char* groupAndPin = highPower->valuestring;
				//char group = groupAndPin[0];//引脚组
        u8 pin = 0;//引脚
        u8 numBit = 0;
        while(*groupAndPin != '\0'){
           if(*groupAndPin >= '0' && *groupAndPin <= '9'){
               numBit++;
					 }
           groupAndPin++;
        }
        groupAndPin = highPower->valuestring;
        if(numBit == 2){
           pin += (groupAndPin[1] - '0') * 10;
           pin += (groupAndPin[2] - '0');
        }else{
           pin += (groupAndPin[1] - '0');
        }
				//直接处理
				controlBit |= pin;
			}
		}
		lowerAndHighPowerControl |= controlBit;
		lowerAndHighPowerBit |= controlBit;
	}
	gpio_output_control('C',0,lowerAndHighPowerControl,1);//开启继电器
	gpio_output_control('D',0,lowerAndHighPowerBit,1);//输入电平
	
	//进行控制端开启后,需要延时,延时后进行检测端的解析
	delay_ms(3000);//延时3秒
	
	//最后解析nodeToCheck
	if(nodeToCheck){
		for(int i = 0;i < cJSON_GetArraySize(nodeToCheck);i++){
			cJSON* check = cJSON_GetArrayItem(nodeToCheck,i);
			if(cJSON_String == check->type){
				//获取端口
				char* groupAndPin = check->valuestring;
				//char group = groupAndPin[0];//引脚组
        u8 pin = 0;//引脚
        u8 numBit = 0;
        while(*groupAndPin != '\0'){
           if(*groupAndPin >= '0' && *groupAndPin <= '9'){
               numBit++;
					 }
           groupAndPin++;
        }
        groupAndPin = check->valuestring;
        if(numBit == 2){
           pin += (groupAndPin[1] - '0') * 10;
           pin += (groupAndPin[2] - '0');
        }else{
           pin += (groupAndPin[1] - '0');
        }
				//直接处理
				if(pin <= 8){
					//处理普通输入
					gpio_input_data_process(pin,0);
				}else{
					//处理模拟输入
					gpio_input_data_process(pin,1);
				}
			}
		}
	}
	
	//处理完成后删除cJSON对象,释放空间
	cJSON_Delete(data);//后面的4个对象是data中的,所以会连带删除.
}


/*
gpio_group : A~B,表示需要控制的gpio的组
gpio_pin : 0~16,表示需要控制的gpio的引脚
gpio_status : 0 / 1 ,表示需要控制的gpio引脚的状态,输出高电平还是低电平.
*/
void gpio_output_control(char gpio_group,u8 gpio_pin,u8 gpio_status,u8 isBinaryData){
	//输出组只有AB;CD都是输入组
	if(!isBinaryData){
		//如果给的数据不是二进制位
		switch(gpio_group){
		case 'A':
			gpioa_control(gpio_pin,gpio_status);
			break;
		case 'B':
			gpiob_control(gpio_pin,gpio_status);
			break;
		}
	}else{
		//如果给的数据是二进制位
		switch(gpio_group){
			case 'C':
				gpio_a3a8_a11a12_binaryDataSet(gpio_status);
			break;
			case 'D':
				gpio_a13a15_b0b4_binaryDataSet(gpio_status);
			break;
		}
	}
}

/*
gpio_pin : 8/16,表示在基础输入模式下需要处理的gpio的引脚,
					 0/1,表示在模拟输入模式下需要获取的adc1/2的数据
process_type : 0 / 1 ,表示需要处理的类型,0表示为普通输入,1表示模拟输入.
*/
void gpio_input_data_process(u8 gpio_pin,u8 process_type){
	if(process_type == 0){
		//普通输入模式处理
		if(gpio_pin <= 8){
			//8就是获取0~7的数据
			sendGpiocData = get_gpioc08_data();
		}else{
			//16就是获取8~15的数据
			sendGpiocData = get_gpioc09_data();
		}
	}else if(process_type == 1){
		//模拟输入模式处理
		u16* temp = get_adc(gpio_pin);
		for(u8 i = 0;i < 500;i++){
			sendAdcData[i] = *temp;
			temp++;
		}
	}
}

void gpio_a3a8_a11a12_binaryDataSet(u8 binaryData){
	//根据二进制位操作A3-A8,A11-A12这8个IO
	//A3-A8,A11-A12 : 0000_0000;
	u8 a = binaryData & 1;
	if(a){
		GPIO_SetBits(GPIOA,GPIO_Pin_12);
	}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	}
	a = binaryData & (1 << 1);
	if(a){
		GPIO_SetBits(GPIOA,GPIO_Pin_11);
	}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	}
	a = binaryData & (1 << 2);
	if(a){
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
	}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	}
	a = binaryData & (1 << 3);
	if(a){
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
	}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	}
	a = binaryData & (1 << 4);
	if(a){
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
	}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	}
	a = binaryData & (1 << 5);
	if(a){
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
	}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	}
	a = binaryData & (1 << 6);
	if(a){
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
	}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	}
	a = binaryData & (1 << 7);
	if(a){
		GPIO_SetBits(GPIOA,GPIO_Pin_3);
	}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	}
}

void gpio_a13a15_b0b4_binaryDataSet(u8 binaryData){
	//根据二进制位操作A13-A15,B0-B4这8个IO
	//A13-A15,B0-B4 : 0000_0000
	u8 a = binaryData & 1;
	if(a){
		GPIO_SetBits(GPIOB,GPIO_Pin_4);
	}else{
		GPIO_ResetBits(GPIOB,GPIO_Pin_4);
	}
	a = binaryData & (1 << 1);
	if(a){
		GPIO_SetBits(GPIOB,GPIO_Pin_3);
	}else{
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);
	}
	a = binaryData & (1 << 2);
	if(a){
		GPIO_SetBits(GPIOB,GPIO_Pin_2);
	}else{
		GPIO_ResetBits(GPIOB,GPIO_Pin_2);
	}
	a = binaryData & (1 << 3);
	if(a){
		GPIO_SetBits(GPIOB,GPIO_Pin_1);
	}else{
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	}
	a = binaryData & (1 << 4);
	if(a){
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
	}else{
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	}
	a = binaryData & (1 << 5);
	if(a){
		GPIO_SetBits(GPIOA,GPIO_Pin_15);
	}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_15);
	}
	a = binaryData & (1 << 6);
	if(a){
		GPIO_SetBits(GPIOA,GPIO_Pin_14);
	}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_14);
	}
	a = binaryData & (1 << 7);
	if(a){
		GPIO_SetBits(GPIOA,GPIO_Pin_13);
	}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_13);
	}
}


void gpioa_control(u8 pin,u8 gpio_status){
	switch(pin){
		case 0:
			if(gpio_status){
				GPIO_SetBits(GPIOA,GPIO_Pin_0);
			}else{
				GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			}
		break;
		case 1:
			if(gpio_status){
				GPIO_SetBits(GPIOA,GPIO_Pin_1);
			}else{
				GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			}
		break;
		case 2:
			if(gpio_status){
				GPIO_SetBits(GPIOA,GPIO_Pin_2);
			}else{
				GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			}
		break;
		case 3:
			if(gpio_status){
				GPIO_SetBits(GPIOA,GPIO_Pin_3);
			}else{
				GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			}
		break;
		case 4:
			if(gpio_status){
				GPIO_SetBits(GPIOA,GPIO_Pin_4);
			}else{
				GPIO_ResetBits(GPIOA,GPIO_Pin_4);
			}
		break;
		case 5:
			if(gpio_status){
				GPIO_SetBits(GPIOA,GPIO_Pin_5);
			}else{
				GPIO_ResetBits(GPIOA,GPIO_Pin_5);
			}
		break;
		case 6:
			if(gpio_status){
				GPIO_SetBits(GPIOA,GPIO_Pin_6);
			}else{
				GPIO_ResetBits(GPIOA,GPIO_Pin_6);
			}
		break;
		case 7:
			if(gpio_status){
				GPIO_SetBits(GPIOA,GPIO_Pin_7);
			}else{
				GPIO_ResetBits(GPIOA,GPIO_Pin_7);
			}
		break;
		case 8:
			if(gpio_status){
				GPIO_SetBits(GPIOA,GPIO_Pin_8);
			}else{
				GPIO_ResetBits(GPIOA,GPIO_Pin_8);
			}
		break;
		case 11:
			if(gpio_status){
				GPIO_SetBits(GPIOA,GPIO_Pin_11);
			}else{
				GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			}
		break;
		case 12:
			if(gpio_status){
				GPIO_SetBits(GPIOA,GPIO_Pin_12);
			}else{
				GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			}
		break;
		case 13:
			if(gpio_status){
				GPIO_SetBits(GPIOA,GPIO_Pin_13);
			}else{
				GPIO_ResetBits(GPIOA,GPIO_Pin_13);
			}
		break;
		case 14:
			if(gpio_status){
				GPIO_SetBits(GPIOA,GPIO_Pin_14);
			}else{
				GPIO_ResetBits(GPIOA,GPIO_Pin_14);
			}
		break;
		case 15:
			if(gpio_status){
				GPIO_SetBits(GPIOA,GPIO_Pin_15);
			}else{
				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
			}
		break;
	}
}

void gpiob_control(u8 pin,u8 gpio_status){
	switch(pin){
		case 0:
			if(gpio_status){
				GPIO_SetBits(GPIOB,GPIO_Pin_0);
			}else{
				GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			}
		break;
		case 1:
			if(gpio_status){
				GPIO_SetBits(GPIOB,GPIO_Pin_1);
			}else{
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);
			}
		break;
		case 2:
			if(gpio_status){
				GPIO_SetBits(GPIOB,GPIO_Pin_2);
			}else{
				GPIO_ResetBits(GPIOB,GPIO_Pin_2);
			}
		break;
		case 3:
			if(gpio_status){
				GPIO_SetBits(GPIOB,GPIO_Pin_3);
			}else{
				GPIO_ResetBits(GPIOB,GPIO_Pin_3);
			}
		break;
		case 4:
			if(gpio_status){
				GPIO_SetBits(GPIOB,GPIO_Pin_4);
			}else{
				GPIO_ResetBits(GPIOB,GPIO_Pin_4);
			}
		break;
		case 5:
			if(gpio_status){
				GPIO_SetBits(GPIOB,GPIO_Pin_5);
			}else{
				GPIO_ResetBits(GPIOB,GPIO_Pin_5);
			}
		break;
		case 6:
			if(gpio_status){
				GPIO_SetBits(GPIOB,GPIO_Pin_6);
			}else{
				GPIO_ResetBits(GPIOB,GPIO_Pin_6);
			}
		break;
		case 7:
			if(gpio_status){
				GPIO_SetBits(GPIOB,GPIO_Pin_7);
			}else{
				GPIO_ResetBits(GPIOB,GPIO_Pin_7);
			}
		break;
		case 8:
			if(gpio_status){
				GPIO_SetBits(GPIOB,GPIO_Pin_8);
			}else{
				GPIO_ResetBits(GPIOB,GPIO_Pin_8);
			}
		break;
		case 9:
			if(gpio_status){
				GPIO_SetBits(GPIOB,GPIO_Pin_9);
			}else{
				GPIO_ResetBits(GPIOB,GPIO_Pin_9);
			}
		break;
		case 12:
			if(gpio_status){
				GPIO_SetBits(GPIOB,GPIO_Pin_12);
			}else{
				GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			}
		break;
		case 13:
			if(gpio_status){
				GPIO_SetBits(GPIOB,GPIO_Pin_13);
			}else{
				GPIO_ResetBits(GPIOB,GPIO_Pin_13);
			}
		break;
		case 14:
			if(gpio_status){
				GPIO_SetBits(GPIOB,GPIO_Pin_14);
			}else{
				GPIO_ResetBits(GPIOB,GPIO_Pin_14);
			}
		break;
		case 15:
			if(gpio_status){
				GPIO_SetBits(GPIOB,GPIO_Pin_15);
			}else{
				GPIO_ResetBits(GPIOB,GPIO_Pin_15);
			}
		break;
	}
}

u8 get_gpioc_data(u8 pin){
	switch(pin){
		case 0:
			return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0);
		case 1:
			return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1);
		case 2:
			return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2);
		case 3:
			return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3);
		case 4:
			return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4);
		case 5:
			return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5);
		case 6:
			return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6);
		case 7:
			return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7);
		case 8:
			return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8);
		case 9:
			return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9);
		case 10:
			return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10);
		case 11:
			return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11);
		case 12:
			return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12);
		case 13:
			return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13);
		case 14:
			return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14);
		case 15:
			return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15);
		default:
			return 2;
	}
}

u8 get_gpioc08_data(){
	u16 readData = GPIO_ReadInputData(GPIOC);//读取C0~C15的数据,0~7在高八位
	return (readData >> 8) & 0xff;//取高八位
}

u8 get_gpioc09_data(){
	u16 readData = GPIO_ReadInputData(GPIOC);////读取C0~C15的数据,8~15在低八位
	return readData & 0xff;//取低八位
}

u16* get_adc(u8 adcPort){
	if(adcPort == 9){
		return adc1_data;
	}
	return adc2_data;
}

