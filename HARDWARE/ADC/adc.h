#ifndef __ADC_H
#define __ADC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ADC��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/4/11
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

void MY_ADC1_Init(void); 				//ADCͨ����ʼ��
void MY_ADC2_Init(void); 
void MY_ADC3_Init(void); 
u16  Get_Adc1(u32 ch); 
u16  Get_Adc2(u32 ch); 
u16  Get_Adc3(u32 ch); 		        //���ĳ��ͨ��ֵ 
u16 Get_Adc1_Average(u32 ch,u8 times,u8 flutter_free);
u16 Get_Adc2_Average(u32 ch,u8 times,u8 flutter_free);
u16 Get_Adc3_Average(u32 ch,u8 times,u8 flutter_free);//�õ�ĳ��ͨ����������������ƽ��ֵ
#endif 
