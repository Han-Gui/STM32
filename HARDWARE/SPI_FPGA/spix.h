#ifndef __SPIX_H
#define __SPIX_H 			   
#include <sys.h>

#define SPI_PORT 	GPIOE
#define SPI_CS   	GPIO_PIN_0
#define SPI_SCLK 	GPIO_PIN_1
#define SPI_MISO 	GPIO_PIN_2
#define SPI_MOSI 	GPIO_PIN_3 //ùùùù
#define SPI_FLAG 	GPIO_PIN_4
#define RCC_SPI  	RCC_AHB1Periph_GPIOE

void SPIx_Init(void);
uint64_t SPIx_SendReadByte36(uint64_t byte);
uint64_t SPIx_SendReadByte16(uint64_t byte);

#endif
