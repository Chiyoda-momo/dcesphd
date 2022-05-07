//�������ݴ���Ͷ˿ڿ���
#include<dataprocessing.h>
#include<stdio.h>
#include<delay.h>

u8* esp8266_data = NULL; // �ӷ���˻�ȡ��JSON�ַ���
u8 haveData = 0; // �Ƿ������ݱ�־λ

void set_data(u8* data){
	esp8266_data = data;
	processOK = 1;
	haveData = 1;
}

/**
esp8266_data��ʽ: 
{lowerPowerTo : ["c5",...],powerToNode : ["p2",...],highPowerTo : ["c4",...],nodeToCheck : ["c1",...]}
Ϊһ��JSON�ַ�����ʽ,��ʹ��cJSON���н���
*/
void data_processing(){
	if(!esp8266_data){
		//��������ǿյ�,�ͽ���
		haveData = 0;
		processOK = 0;//����������
		return;
	}
	//��Ϊ�վ�������
	cJSON* data = cJSON_Parse((char*)esp8266_data);//���ݻ�ȡ�����ݹ���cJSON����
	if(!data){
		//��ȡ��JSON�ַ���������!
		processOK = 0;//����������
		return;
	}
	//�ĸ�����cJSON�������.
	cJSON* lowerPowerTo = cJSON_GetObjectItem(data,"lowerPowerTo");
	cJSON* powerToNode = cJSON_GetObjectItem(data,"powerToNode");
	cJSON* highPowerTo = cJSON_GetObjectItem(data,"highPowerTo");
	cJSON* nodeToCheck = cJSON_GetObjectItem(data,"nodeToCheck");
	//�ֱ����
	//���ĸ�������������
	//���Ƚ���powerToNode
	//��ǰpowerToNodeֻ������һ������,ֱ�Ӱ������Ƚ�
	if(powerToNode){
		for(int i = 0;i < cJSON_GetArraySize(powerToNode);i++){
			cJSON* power = cJSON_GetArrayItem(powerToNode,i);
			if(cJSON_String == power->type){
				//��ȡ�˿�
				char* groupAndPin = power->valuestring;
				//char group = groupAndPin[0];//P,��ʾ����
        u8 pin = 0;//����
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
				//ֱ�Ӵ���
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
	//����lowerPowerTo��highPowerTo
	u8 lowerAndHighPowerControl = 0;
	u8 lowerAndHighPowerBit = 0;
	if(lowerPowerTo){
		u8 controlBit = 0;
		for(int i = 0;i < cJSON_GetArraySize(lowerPowerTo);i++){
			cJSON* lowerPower = cJSON_GetArrayItem(lowerPowerTo,i);
			if(cJSON_String == lowerPower->type){
				//��ȡ�˿�
				char* groupAndPin = lowerPower->valuestring;
				//char group = groupAndPin[0];//C,��ʾ����
        u8 pin = 0;//����
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
		//ֱ�Ӵ���
		//A3~A8,A11~A12 : 0000_0000 �̵�������
		//A13~A15,B0~B4 : 0000_0000 ��ƽ����
	}
	if(highPowerTo){
		u8 controlBit = 0;
		for(int i = 0;i < cJSON_GetArraySize(highPowerTo);i++){
			cJSON* highPower = cJSON_GetArrayItem(highPowerTo,i);
			if(cJSON_String == highPower->type){
				//��ȡ�˿�
				char* groupAndPin = highPower->valuestring;
				//char group = groupAndPin[0];//������
        u8 pin = 0;//����
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
				//ֱ�Ӵ���
				controlBit |= pin;
			}
		}
		lowerAndHighPowerControl |= controlBit;
		lowerAndHighPowerBit |= controlBit;
	}
	gpio_output_control('C',0,lowerAndHighPowerControl,1);//�����̵���
	gpio_output_control('D',0,lowerAndHighPowerBit,1);//�����ƽ
	
	//���п��ƶ˿�����,��Ҫ��ʱ,��ʱ����м��˵Ľ���
	delay_ms(3000);//��ʱ3��
	
	//������nodeToCheck
	if(nodeToCheck){
		for(int i = 0;i < cJSON_GetArraySize(nodeToCheck);i++){
			cJSON* check = cJSON_GetArrayItem(nodeToCheck,i);
			if(cJSON_String == check->type){
				//��ȡ�˿�
				char* groupAndPin = check->valuestring;
				//char group = groupAndPin[0];//������
        u8 pin = 0;//����
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
				//ֱ�Ӵ���
				if(pin <= 8){
					//������ͨ����
					gpio_input_data_process(pin,0);
				}else{
					//����ģ������
					gpio_input_data_process(pin,1);
				}
			}
		}
	}
	
	//������ɺ�ɾ��cJSON����,�ͷſռ�
	cJSON_Delete(data);//�����4��������data�е�,���Ի�����ɾ��.
}


/*
gpio_group : A~B,��ʾ��Ҫ���Ƶ�gpio����
gpio_pin : 0~16,��ʾ��Ҫ���Ƶ�gpio������
gpio_status : 0 / 1 ,��ʾ��Ҫ���Ƶ�gpio���ŵ�״̬,����ߵ�ƽ���ǵ͵�ƽ.
*/
void gpio_output_control(char gpio_group,u8 gpio_pin,u8 gpio_status,u8 isBinaryData){
	//�����ֻ��AB;CD����������
	if(!isBinaryData){
		//����������ݲ��Ƕ�����λ
		switch(gpio_group){
		case 'A':
			gpioa_control(gpio_pin,gpio_status);
			break;
		case 'B':
			gpiob_control(gpio_pin,gpio_status);
			break;
		}
	}else{
		//������������Ƕ�����λ
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
gpio_pin : 8/16,��ʾ�ڻ�������ģʽ����Ҫ�����gpio������,
					 0/1,��ʾ��ģ������ģʽ����Ҫ��ȡ��adc1/2������
process_type : 0 / 1 ,��ʾ��Ҫ���������,0��ʾΪ��ͨ����,1��ʾģ������.
*/
void gpio_input_data_process(u8 gpio_pin,u8 process_type){
	if(process_type == 0){
		//��ͨ����ģʽ����
		if(gpio_pin <= 8){
			//8���ǻ�ȡ0~7������
			sendGpiocData = get_gpioc08_data();
		}else{
			//16���ǻ�ȡ8~15������
			sendGpiocData = get_gpioc09_data();
		}
	}else if(process_type == 1){
		//ģ������ģʽ����
		u16* temp = get_adc(gpio_pin);
		for(u8 i = 0;i < 500;i++){
			sendAdcData[i] = *temp;
			temp++;
		}
	}
}

void gpio_a3a8_a11a12_binaryDataSet(u8 binaryData){
	//���ݶ�����λ����A3-A8,A11-A12��8��IO
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
	//���ݶ�����λ����A13-A15,B0-B4��8��IO
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
	u16 readData = GPIO_ReadInputData(GPIOC);//��ȡC0~C15������,0~7�ڸ߰�λ
	return (readData >> 8) & 0xff;//ȡ�߰�λ
}

u8 get_gpioc09_data(){
	u16 readData = GPIO_ReadInputData(GPIOC);////��ȡC0~C15������,8~15�ڵͰ�λ
	return readData & 0xff;//ȡ�Ͱ�λ
}

u16* get_adc(u8 adcPort){
	if(adcPort == 9){
		return adc1_data;
	}
	return adc2_data;
}

