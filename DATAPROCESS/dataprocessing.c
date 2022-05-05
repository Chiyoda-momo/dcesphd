//进行数据处理和端口控制
#include<dataprocessing.h>


void get_data(u8* data){

}

void data_processing(){

}


/*
gpio_group : A~D,表示需要控制的gpio的组
gpio_pin : 0~16,表示需要控制的gpio的引脚
gpio_status : 0 / 1 ,表示需要控制的gpio引脚的状态,输出高电平还是低电平.
*/
void gpio_output_control(char gpio_group,u8 gpio_pin,u8 gpio_status){

}

/*
gpio_group : A~D,表示需要处理的gpio的组
gpio_pin : 0~16,表示需要处理的gpio的引脚
process_type : 0 / 1 ,表示需要处理的类型,0表示为普通输入,1表示模拟输入.
*/
void gpio_input_data_process(char gpio_group,u8 gpio_pin,u8 process_type){


}
