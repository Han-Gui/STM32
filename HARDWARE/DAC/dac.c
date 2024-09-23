#include "dac.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//DAC驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/4/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

DAC_HandleTypeDef DAC1_Handler;//DAC句柄
DAC_HandleTypeDef DAC2_Handler;
//初始化DAC
void DAC1_Init(void)
{
    DAC_ChannelConfTypeDef DACCH1_Config;
    
    DAC1_Handler.Instance=DAC;
    HAL_DAC_Init(&DAC1_Handler);                 //初始化DAC
    
    DACCH1_Config.DAC_Trigger=DAC_TRIGGER_NONE;             //不使用触发功能
    DACCH1_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_DISABLE;//DAC1输出缓冲关闭
    HAL_DAC_ConfigChannel(&DAC1_Handler,&DACCH1_Config,DAC_CHANNEL_1);//DAC通道1配置
    
    HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_1);  //开启DAC通道1
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

//DAC底层驱动，时钟配置，引脚 配置
//此函数会被HAL_DAC_Init()调用
//hdac:DAC句柄
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

//设置通道1输出电压
//vol:0~3300,代表0~3.3V
void DAC1_Set_Vol(u16 vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
    HAL_DAC_SetValue(&DAC1_Handler,DAC_CHANNEL_1,DAC_ALIGN_12B_R,temp);//12位右对齐数据格式设置DAC值
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
    float incval;           /* 递增量 */
    
    if((maxval + 1) <= samples)return ; /* 数据不合法 */
        
    incval = (maxval + 1) / (samples / 2);  /* 计算递增量 */
    
    for(j = 0; j < n; j++)
    { 
        DAC1->DHR12R1 = 0;  /* 先输出0 */
        for(i = 0; i < (samples / 2); i++)  /* 输出上升沿 */
        { 
             DAC1->DHR12R1 +=  incval;
             delay_us(dt);
        } 
        for(i = 0; i < (samples / 2); i++)  /* 输出下降沿 */
        {
             DAC1->DHR12R1 -=  incval;
             delay_us(dt);
        }
    }
}
