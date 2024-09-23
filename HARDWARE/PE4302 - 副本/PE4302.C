#include "PE4302.H"

/**************************************************************** 
函数名称: PE_GPIO_Init 
功    能: 管脚初始化 
参    数: 无
返回值  : 无 
*****************************************************************/ 
void PE4302_Init(void)
{
	
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();               //开启GPIOA时钟
	
	GPIO_Initure.Pin=PE4302_DAT_Pin|PE4302_CLK_Pin|PE4302_LE_Pin;		//PE4302三个引脚
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;      //推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;							//上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     		//高速
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);

	
	PE_LE_0_DIS;
	PE_CLK_0;
	PE_DAT_0;
}
/**************************************************************** 
函数名称: PE4302_Set 
功    能: 衰减值设置 
参    数: db，范围：0~63，对应衰减0~31.5dB
返回值  : 无 
*****************************************************************/ 
void PE4302_Set(unsigned int db)
{
  unsigned int i;
  unsigned int W_DB;
	W_DB = db;
	PE_LE_0_DIS;
	PE_CLK_0;
	for(i = 0; i < 6;i++)
	{
		if (0x20 == (W_DB & 0X20))
		{	
		  PE_DAT_1;
		}
		else
		{
			PE_DAT_0;
		}
		  delay_us(500);
		  PE_CLK_1;
			delay_us(500);
			PE_CLK_0;
			delay_us(500);
		  W_DB = W_DB << 1;
     		
  }
	   PE_LE_0_EN;
     delay_us(500);
	   PE_LE_0_DIS;
     PE_DAT_0;
}

