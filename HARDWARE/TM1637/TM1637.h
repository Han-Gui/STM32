#ifndef __TM1637_H
#define __TM1637_H

#include "sys.h"

#define TM1640_SCLK_PIN  GPIO_PIN_6
#define TM1640_DIN_PIN   GPIO_PIN_8

#define TM1640_LEDPORT   0xC8

#define I2C_SCL  PBout(6) //定义模拟IIC总线的时钟线
#define I2C_SDA  PBout(8) //定义模拟IIC总线的数据线

void TM1637_Init(void);
void TM1637DSP_Control(unsigned char enable_display, unsigned char bright);
void TM1637(void);
void TM1637_init(void);
void TM1637_show_digit(unsigned char pos, unsigned char digit, unsigned char with_dot);
void TM1637_showint(u16 data);
void TM1637_display(u16 data);
void TM1637_write_display_byte(unsigned char addr, unsigned char segments);
void TM1637_clear(void);
u8 Confirm_Bin(u8* Bin_data, u8 num, char* refer);
u8* BinToHex(u8* Bin_data, u8 num);
u16 BinToDec(u8* Bin_data, u8 num);

#endif

