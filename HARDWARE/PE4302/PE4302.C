#include "PE4302.H"

/**************************************************************** 
函数名称: PE_GPIO_Init 
功    能: 管脚初始化 
参    数: 无
返回值  : 无 
*****************************************************************/ 

void PE4302_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOC clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitStructure.Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Set initial output values to high */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11, GPIO_PIN_SET);
}

/**************************************************************** 
函数名称: PE4302_Set 
功    能: 衰减值设置 
参    数: db，范围：0~63，对应衰减0~31.5dB
返回值  : 无 
*****************************************************************/ 
void PE4302_Set(unsigned char db)
{
  unsigned char i;
  unsigned char W_DB;
  db = db&0X3F;
	
  PE_CLK_0;
	delay_us(500);
  PE_LE_0_EN;
	delay_us(500);
  for(i = 0; i < 6;i++)
  {
    W_DB = (db>>5);
    if(W_DB == 1)
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
    db = (db << 1)&0X3F;
    PE_CLK_0;
		delay_us(500);
  }
  PE_LE_0_DIS;
	delay_us(500);
	PE_DAT_0;
}

