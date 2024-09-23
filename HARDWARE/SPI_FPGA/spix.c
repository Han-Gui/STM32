#include "spix.h"
#include "delay.h"
#include "led.h"

void SPIx_Init(void)
{
  GPIO_InitTypeDef SPI_GPIO_InitStructure;
  __HAL_RCC_GPIOE_CLK_ENABLE();     
	
  SPI_GPIO_InitStructure.Pin =  SPI_MISO;//MOSI
  SPI_GPIO_InitStructure.Mode =  GPIO_MODE_INPUT;  
  SPI_GPIO_InitStructure.Pull = GPIO_PULLUP;	//·输入模式存疑·
  SPI_GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(SPI_PORT, &SPI_GPIO_InitStructure);
	
  /*SPI_GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;//MOSI
  SPI_GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;   //·输入模式存疑·
  SPI_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(SPI_PORT, &SPI_GPIO_InitStructure);*/

	SPI_GPIO_InitStructure.Pin = SPI_CS|SPI_SCLK|SPI_FLAG;//CS,SCLK,MISO
  SPI_GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	//SPI_GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  SPI_GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;				//·翻转速度存疑·  原为50m
  HAL_GPIO_Init(SPI_PORT, &SPI_GPIO_InitStructure);	
}


uint64_t SPIx_SendReadByte36(uint64_t byte)
{
  uint64_t DataSend = byte; 
  uint64_t DataRead = 0;
  int i = 0;
  u8 SDI = 0;	
	LED0=!LED0;
  HAL_GPIO_WritePin(SPI_PORT,SPI_CS,GPIO_PIN_SET);
  //GPIO_SetBits(SPI_PORT,SPI_CS);//cs = 1          
  delay_us(1);
  HAL_GPIO_WritePin(SPI_PORT,SPI_CS,GPIO_PIN_RESET);
  //GPIO_ResetBits(SPI_PORT,SPI_CS);//cs = 0
	delay_us(1);
	
  for (i = 0; i < 36; i++)
  {
    HAL_GPIO_WritePin(SPI_PORT,SPI_SCLK,GPIO_PIN_SET);
    //GPIO_SetBits(SPI_PORT,SPI_SCLK);//sclk = 1    
    delay_us(1);
    /*if ((DataSend<<i)&0x800000000)		//第16位为1
    {
      GPIO_SetBits(SPI_PORT,SPI_MISO); //output = 1 	
    }
    else
    {
      GPIO_ResetBits(SPI_PORT,SPI_MISO);//output = 0
    }
    */
    DataRead <<= 1;
    HAL_GPIO_WritePin(SPI_PORT,SPI_SCLK,GPIO_PIN_RESET);
    //GPIO_ResetBits(SPI_PORT,SPI_SCLK);//sclk = 0;
    delay_us(1);
		
    SDI = HAL_GPIO_ReadPin(SPI_PORT, SPI_MISO);
    //SDI = GPIO_ReadInputDataBit(SPI_PORT, SPI_MISO);//*****
		
    if(SDI)
		{
      DataRead |= 0x0001;	
		}
    else
		{
			DataRead &= ~0x0001;
		}
  }
  HAL_GPIO_WritePin(SPI_PORT,SPI_CS,GPIO_PIN_SET);
  //GPIO_SetBits(SPI_PORT,SPI_CS); // cs = 1
	delay_us(1);
  return DataRead;    
}

uint64_t SPIx_SendReadByte16(uint64_t byte)
{
  uint64_t DataSend = byte; 
  uint64_t DataRead = 0;
  int i = 0;
  u8 SDI = 0;
  HAL_GPIO_WritePin(SPI_PORT,SPI_CS,GPIO_PIN_SET);
  //GPIO_SetBits(SPI_PORT,SPI_CS);//cs = 1
  delay_us(1000);
  HAL_GPIO_WritePin(SPI_PORT,SPI_CS,GPIO_PIN_RESET);
  //GPIO_ResetBits(SPI_PORT,SPI_CS);//cs = 0
  delay_us(1);
  for (i = 0; i < 16; i++)
  {
    HAL_GPIO_WritePin(SPI_PORT,SPI_SCLK,GPIO_PIN_SET);
    //GPIO_SetBits(SPI_PORT,SPI_SCLK);//sclk = 1    
    delay_us(1);
    if ((DataSend<<i)&0x8000) 
    {
      HAL_GPIO_WritePin(SPI_PORT,SPI_MISO,GPIO_PIN_SET);
      //GPIO_SetBits(SPI_PORT,SPI_MISO); //output = 1 
    }
    else
    {
      HAL_GPIO_WritePin(SPI_PORT,SPI_MISO,GPIO_PIN_RESET);
      //GPIO_ResetBits(SPI_PORT, SPI_MISO);//output = 0
    }
    
    DataRead <<= 1;
    HAL_GPIO_WritePin(SPI_PORT,SPI_SCLK,GPIO_PIN_RESET);
    //GPIO_ResetBits(SPI_PORT, SPI_SCLK);//sclk = 0;
    delay_us(1);
    SDI = HAL_GPIO_ReadPin(SPI_PORT, SPI_MISO);
    //SDI = GPIO_ReadInputDataBit(SPI_PORT, SPI_MOSI);//read 
    if(SDI)
      DataRead |= 0x0001;
    else
    DataRead &= ~0x0001;
  }
  HAL_GPIO_WritePin(SPI_PORT,SPI_CS,GPIO_PIN_RESET);
  //GPIO_SetBits(SPI_PORT,SPI_CS); // cs = 1
  delay_us(1);
  return DataRead;    
}
