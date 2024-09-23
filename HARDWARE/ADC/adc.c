#include "adc.h"
#include "delay.h"
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

ADC_HandleTypeDef ADC1_Handler;
ADC_HandleTypeDef ADC2_Handler;
ADC_HandleTypeDef ADC3_Handler;//ADC句柄

//初始化ADC
//ch: ADC_channels 
//通道值 0~16取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
void MY_ADC1_Init(void)
{ 
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4分频，ADCCLK=PCLK2/4=84/4=21MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12位模式
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
    ADC1_Handler.Init.ScanConvMode=DISABLE;                      //非扫描模式
    ADC1_Handler.Init.EOCSelection=DISABLE;                      //关闭EOC中断
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                //关闭连续转换
    ADC1_Handler.Init.NbrOfConversion=1;                         //1个转换在规则序列中 也就是只转换规则序列1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //不连续采样通道数为0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //软件触发
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//使用软件触发
    ADC1_Handler.Init.DMAContinuousRequests=DISABLE;             //关闭DMA请求
    HAL_ADC_Init(&ADC1_Handler);                                 //初始化 
}
void MY_ADC2_Init(void)
{ 
		ADC2_Handler.Instance=ADC2;
    ADC2_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4分频，ADCCLK=PCLK2/4=84/4=21MHZ
    ADC2_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12位模式
    ADC2_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
    ADC2_Handler.Init.ScanConvMode=DISABLE;                      //非扫描模式
    ADC2_Handler.Init.EOCSelection=DISABLE;                      //关闭EOC中断
    ADC2_Handler.Init.ContinuousConvMode=DISABLE;                //关闭连续转换
    ADC2_Handler.Init.NbrOfConversion=1;                         //1个转换在规则序列中 也就是只转换规则序列1 
    ADC2_Handler.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
    ADC2_Handler.Init.NbrOfDiscConversion=0;                     //不连续采样通道数为0
    ADC2_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //软件触发
    ADC2_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//使用软件触发
    ADC2_Handler.Init.DMAContinuousRequests=DISABLE;             //关闭DMA请求
    HAL_ADC_Init(&ADC2_Handler);                                 //初始化 
}

void MY_ADC3_Init(void)
{ 
		ADC3_Handler.Instance=ADC3;
    ADC3_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4分频，ADCCLK=PCLK2/4=84/4=21MHZ
    ADC3_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12位模式
    ADC3_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
    ADC3_Handler.Init.ScanConvMode=DISABLE;                      //非扫描模式
    ADC3_Handler.Init.EOCSelection=DISABLE;                      //关闭EOC中断
    ADC3_Handler.Init.ContinuousConvMode=DISABLE;                //关闭连续转换
    ADC3_Handler.Init.NbrOfConversion=1;                         //1个转换在规则序列中 也就是只转换规则序列1 
    ADC3_Handler.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
    ADC3_Handler.Init.NbrOfDiscConversion=0;                     //不连续采样通道数为0
    ADC3_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //软件触发
    ADC3_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//使用软件触发
    ADC3_Handler.Init.DMAContinuousRequests=DISABLE;             //关闭DMA请求
    HAL_ADC_Init(&ADC3_Handler);                                 //初始化 
}

//ADC底层驱动，引脚配置，时钟使能
//此函数会被HAL_ADC_Init()调用
//hadc:ADC句柄
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
		GPIO_InitTypeDef GPIO_Initure_ADC1;
		GPIO_InitTypeDef GPIO_Initure_ADC2;
		GPIO_InitTypeDef GPIO_Initure_ADC3;
	
    __HAL_RCC_ADC1_CLK_ENABLE();            //使能ADC1时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	
    GPIO_Initure_ADC1.Pin=GPIO_PIN_7;            //PA7
    GPIO_Initure_ADC1.Mode=GPIO_MODE_ANALOG;     //模拟
    GPIO_Initure_ADC1.Pull=GPIO_NOPULL;          //不带上下拉
    HAL_GPIO_Init(GPIOA,&GPIO_Initure_ADC1);	
	
    __HAL_RCC_ADC2_CLK_ENABLE();            //使能ADC2时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
	
    GPIO_Initure_ADC2.Pin=GPIO_PIN_4;            //PC4
    GPIO_Initure_ADC2.Mode=GPIO_MODE_ANALOG;     //模拟
    GPIO_Initure_ADC2.Pull=GPIO_NOPULL;          //不带上下拉
    HAL_GPIO_Init(GPIOC,&GPIO_Initure_ADC2);	
	
    __HAL_RCC_ADC3_CLK_ENABLE();            //使能ADC3时钟
    __HAL_RCC_GPIOF_CLK_ENABLE();			//开启GPIOF时钟
	
    GPIO_Initure_ADC3.Pin=GPIO_PIN_10;            //PF10
    GPIO_Initure_ADC3.Mode=GPIO_MODE_ANALOG;     //模拟
    GPIO_Initure_ADC3.Pull=GPIO_NOPULL;          //不带上下拉
    HAL_GPIO_Init(GPIOF,&GPIO_Initure_ADC3);
}

//获得ADC值
//ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
//返回值:转换结果

u16 Get_Adc1(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //通道
    ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
    ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //通道配置
	
    HAL_ADC_Start(&ADC1_Handler);                               //开启ADC
	
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //轮询转换
 
	return (u16)HAL_ADC_GetValue(&ADC1_Handler);	        //返回最近一次ADC1规则组的转换结果
}

u16 Get_Adc2(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC2_ChanConf;
    
    ADC2_ChanConf.Channel=ch;                                   //通道
    ADC2_ChanConf.Rank=1;                                       //第1个序列，序列1
    ADC2_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
    ADC2_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&ADC2_Handler,&ADC2_ChanConf);        //通道配置
	
    HAL_ADC_Start(&ADC2_Handler);                               //开启ADC
	
    HAL_ADC_PollForConversion(&ADC2_Handler,10);                //轮询转换
 
	return (u16)HAL_ADC_GetValue(&ADC2_Handler);	        //返回最近一次ADC1规则组的转换结果
}

u16 Get_Adc3(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC3_ChanConf;
    
    ADC3_ChanConf.Channel=ch;                                   //通道
    ADC3_ChanConf.Rank=1;                                       //第1个序列，序列1
    ADC3_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
    ADC3_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&ADC3_Handler,&ADC3_ChanConf);        //通道配置
	
    HAL_ADC_Start(&ADC3_Handler);                               //开启ADC
	
    HAL_ADC_PollForConversion(&ADC3_Handler,10);                //轮询转换
 
	return (u16)HAL_ADC_GetValue(&ADC3_Handler);	        //返回最近一次ADC1规则组的转换结果
}

//获取指定通道的转换值，取times次,然后平均 
//times:获取次数
//返回值:通道ch的times次转换结果平均值
//u16 Get_Adc_Average(u32 ch,u8 times)
//{
//	u32 temp_val=0;
//	u8 t;
//	for(t=0;t<times;t++)
//	{
//		temp_val+=Get_Adc(ch);
//		delay_ms(5);
//	}
//	return temp_val/times;
//} 
//原版例程里的getaverage，多一个消抖过程

u16 Get_Adc1_Average(u32 ch,u8 times,u8 flutter_free)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<flutter_free;t++)
	{
		temp_val+=Get_Adc1(ch);
		delay_ms(5);
	}for(temp_val=0,t=0;t<times;t++)
	{
		temp_val+=Get_Adc1(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 

u16 Get_Adc2_Average(u32 ch,u8 times,u8 flutter_free)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<flutter_free;t++)
	{
		temp_val+=Get_Adc2(ch);
		delay_ms(5);
	}for(temp_val=0,t=0;t<times;t++)
	{
		temp_val+=Get_Adc2(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 

u16 Get_Adc3_Average(u32 ch,u8 times,u8 flutter_free)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<flutter_free;t++)
	{
		temp_val+=Get_Adc3(ch);
		delay_ms(5);
	}for(temp_val=0,t=0;t<times;t++)
	{
		temp_val+=Get_Adc3(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 

