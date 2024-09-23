#ifndef _PE4302_H_
#define _PE4302_H_

#include "sys.h"
#include "delay.h"


//#define RCC_PE4302_CLK		RCC_AHB1Periph_GPIOA
//#define RCC_PE4302_DAT	  RCC_AHB1Periph_GPIOA
//#define RCC_PE4302_DIN		RCC_AHB1Periph_GPIOE
//#define RCC_PE4302_LE		  RCC_AHB1Periph_GPIOA


#define PE4302_CLK_GPIO	  GPIOA
#define PE4302_DAT_GPIO	  GPIOA
//#define PE4302_DOUT_GPIO	GPIOE
#define PE4302_LE_GPIO		GPIOA


#define PE4302_DAT_Pin	  GPIO_PIN_0
#define PE4302_CLK_Pin		GPIO_PIN_1
//#define PE4302_DIN_Pin	GPIO_Pin_3
#define PE4302_LE_Pin		  GPIO_PIN_4


#define PE_DAT         PAout(0)
#define PE_CLK         PAout(1)
#define PE_LE_0        PAout(4)


#define PE_LE_0_EN      PE_LE_0 = 1
#define PE_LE_0_DIS     PE_LE_0 = 0

#define PE_CLK_0        PE_CLK = 0
#define PE_CLK_1        PE_CLK = 1

#define PE_DAT_0        PE_DAT = 0
#define PE_DAT_1        PE_DAT = 1

void PE4302_Init(void);
void PE4302_Set(unsigned int db);
//void PE4302_1_Set(unsigned char db);
#endif
