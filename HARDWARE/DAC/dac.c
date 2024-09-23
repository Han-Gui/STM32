#include "dac.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//DAC��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/4/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

DAC_HandleTypeDef DAC1_Handler;//DAC���
DAC_HandleTypeDef DAC2_Handler;
//��ʼ��DAC
void DAC1_Init(void)
{
    DAC_ChannelConfTypeDef DACCH1_Config;
    
    DAC1_Handler.Instance=DAC;
    HAL_DAC_Init(&DAC1_Handler);                 //��ʼ��DAC
    
    DACCH1_Config.DAC_Trigger=DAC_TRIGGER_NONE;             //��ʹ�ô�������
    DACCH1_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_DISABLE;//DAC1�������ر�
    HAL_DAC_ConfigChannel(&DAC1_Handler,&DACCH1_Config,DAC_CHANNEL_1);//DACͨ��1����
    
    HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_1);  //����DACͨ��1
}

void DAC2_Init(void)
{
    DAC_ChannelConfTypeDef DACCH2_Config;
    
    DAC2_Handler.Instance=DAC;
    HAL_DAC_Init(&DAC2_Handler);                 //???DAC
    
    DACCH2_Config.DAC_Trigger=DAC_TRIGGER_NONE;             //???????
    DACCH2_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_DISABLE;//DAC2??????
    HAL_DAC_ConfigChannel(&DAC2_Handler,&DACCH2_Config,DAC_CHANNEL_2);//DAC??2??
    
    HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_2);  //??DAC??2
}

//DAC�ײ�������ʱ�����ã����� ����
//�˺����ᱻHAL_DAC_Init()����
//hdac:DAC���
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{      
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_DAC_CLK_ENABLE();             //??DAC??
    __HAL_RCC_GPIOA_CLK_ENABLE();			//??GPIOA??
	
    GPIO_Initure.Pin=GPIO_PIN_4|GPIO_PIN_5;            //PA4 PA5
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //??
    GPIO_Initure.Pull=GPIO_NOPULL;          //?????
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

//����ͨ��1�����ѹ
//vol:0~3300,����0~3.3V
void DAC1_Set_Vol(u16 vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
    HAL_DAC_SetValue(&DAC1_Handler,DAC_CHANNEL_1,DAC_ALIGN_12B_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
}

void DAC2_Set_Vol(u16 vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
    HAL_DAC_SetValue(&DAC1_Handler,DAC_CHANNEL_2,DAC_ALIGN_12B_R,temp);//12??????????DAC?
}

void dac_triangular_wave(uint16_t maxval, uint16_t dt, uint16_t samples, uint16_t n)
{
    uint16_t i, j;
    float incval;           /* ������ */
    
    if((maxval + 1) <= samples)return ; /* ���ݲ��Ϸ� */
        
    incval = (maxval + 1) / (samples / 2);  /* ��������� */
    
    for(j = 0; j < n; j++)
    { 
        DAC1->DHR12R1 = 0;  /* �����0 */
        for(i = 0; i < (samples / 2); i++)  /* ��������� */
        { 
             DAC1->DHR12R1 +=  incval;
             delay_us(dt);
        } 
        for(i = 0; i < (samples / 2); i++)  /* ����½��� */
        {
             DAC1->DHR12R1 -=  incval;
             delay_us(dt);
        }
    }
}
