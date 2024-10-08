#ifndef __ADC_H
#define __ADC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//ADC驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/4/11
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

void MY_ADC1_Init(void); 				//ADC通道初始化
void MY_ADC2_Init(void); 
void MY_ADC3_Init(void); 
u16  Get_Adc1(u32 ch); 
u16  Get_Adc2(u32 ch); 
u16  Get_Adc3(u32 ch); 		        //获得某个通道值 
u16 Get_Adc1_Average(u32 ch,u8 times,u8 flutter_free);
u16 Get_Adc2_Average(u32 ch,u8 times,u8 flutter_free);
u16 Get_Adc3_Average(u32 ch,u8 times,u8 flutter_free);//得到某个通道给定次数采样的平均值
#endif 
