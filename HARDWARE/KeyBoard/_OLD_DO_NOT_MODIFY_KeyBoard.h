#ifndef KEYBOARD_H_
#define KEYBOARD_H_
#include "stm32f4xx.h"
#include "stm32f4xx_ll_exti.h"
//#include "sys.h"

//键盘是否镜像（印字正方向时为MIRROR）
#define KEYBOARD_REAL	0
#define KEYBOARD_MIRROR	1

//各行列的GPIO端口和对应按键
#define 	ROW0_GPIO	GPIOB
#define		ROW1_GPIO	GPIOB
#define 	ROW2_GPIO	GPIOB
#define		ROW3_GPIO	GPIOB

#define 	COL0_GPIO	GPIOB
#define 	COL1_GPIO	GPIOG
#define 	COL2_GPIO	GPIOG
#define 	COL3_GPIO	GPIOG

#define 	ROW0_GPIO_BIT	GPIO_PIN_7
#define		ROW1_GPIO_BIT	GPIO_PIN_6
#define 	ROW2_GPIO_BIT	GPIO_PIN_5
#define		ROW3_GPIO_BIT	GPIO_PIN_4

#define 	COL0_GPIO_BIT	GPIO_PIN_3
#define 	COL1_GPIO_BIT	GPIO_PIN_15
#define 	COL2_GPIO_BIT	GPIO_PIN_14
#define 	COL3_GPIO_BIT	GPIO_PIN_13

//将所有行、列置高或置低
#define ROW_H	{ROW0_H;ROW1_H;ROW2_H;ROW3_H;}
#define ROW_L	{ROW0_L;ROW1_L;ROW2_L;ROW3_L;}

#define COL_H	{COL0_H;COL1_H;COL2_H;COL3_H;}
#define COL_L	{COL0_L;COL1_L;COL2_L;COL3_L;}

//将指定行、列置高或置低
#define ROW0_H 	HAL_GPIO_WritePin(ROW0_GPIO,ROW0_GPIO_BIT,GPIO_PIN_SET)
#define ROW1_H 	HAL_GPIO_WritePin(ROW1_GPIO,ROW1_GPIO_BIT,GPIO_PIN_SET)
#define ROW2_H 	HAL_GPIO_WritePin(ROW2_GPIO,ROW2_GPIO_BIT,GPIO_PIN_SET)
#define ROW3_H 	HAL_GPIO_WritePin(ROW3_GPIO,ROW3_GPIO_BIT,GPIO_PIN_SET)

#define ROW0_L 	HAL_GPIO_WritePin(ROW0_GPIO,ROW0_GPIO_BIT,GPIO_PIN_RESET)
#define ROW1_L 	HAL_GPIO_WritePin(ROW1_GPIO,ROW1_GPIO_BIT,GPIO_PIN_RESET)
#define ROW2_L 	HAL_GPIO_WritePin(ROW2_GPIO,ROW2_GPIO_BIT,GPIO_PIN_RESET)
#define ROW3_L 	HAL_GPIO_WritePin(ROW3_GPIO,ROW3_GPIO_BIT,GPIO_PIN_RESET)

#define COL0_H	HAL_GPIO_WritePin(COL0_GPIO,COL0_GPIO_BIT,GPIO_PIN_SET)
#define COL1_H	HAL_GPIO_WritePin(COL1_GPIO,COL1_GPIO_BIT,GPIO_PIN_SET)
#define COL2_H	HAL_GPIO_WritePin(COL2_GPIO,COL2_GPIO_BIT,GPIO_PIN_SET)
#define COL3_H	HAL_GPIO_WritePin(COL3_GPIO,COL3_GPIO_BIT,GPIO_PIN_SET)

#define COL0_L	HAL_GPIO_WritePin(COL0_GPIO,COL0_GPIO_BIT,GPIO_PIN_RESET)
#define COL1_L	HAL_GPIO_WritePin(COL1_GPIO,COL1_GPIO_BIT,GPIO_PIN_RESET)
#define COL2_L	HAL_GPIO_WritePin(COL2_GPIO,COL2_GPIO_BIT,GPIO_PIN_RESET)
#define COL3_L	HAL_GPIO_WritePin(COL3_GPIO,COL3_GPIO_BIT,GPIO_PIN_RESET)

//读取对应行列是否被拉低
#define ROW0	HAL_GPIO_ReadPin(ROW0_GPIO,ROW0_GPIO_BIT) //B7
#define ROW1	HAL_GPIO_ReadPin(ROW1_GPIO,ROW1_GPIO_BIT) //B6
#define ROW2	HAL_GPIO_ReadPin(ROW2_GPIO,ROW2_GPIO_BIT)//B5 
#define ROW3	HAL_GPIO_ReadPin(ROW3_GPIO,ROW3_GPIO_BIT) //B4

#define COL0	HAL_GPIO_ReadPin(COL0_GPIO,COL0_GPIO_BIT)//B3
#define COL1	HAL_GPIO_ReadPin(COL1_GPIO,COL1_GPIO_BIT)//G15
#define COL2	HAL_GPIO_ReadPin(COL2_GPIO,COL2_GPIO_BIT)//G14
#define COL3	HAL_GPIO_ReadPin(COL3_GPIO,COL3_GPIO_BIT)//G13

//按键功能宏定义
//数字和小数点
#define KEY_1
#define KEY_2
#define KEY_3
#define KEY_4
#define KEY_5
#define KEY_6
#define KEY_7
#define KEY_8
#define KEY_9
#define KEY_0
#define KEY_POINT	//小数点

//方向（和数字复用）
#define KEY_UP KEY_2
#define KEY_DOWN KEY_8
#define KEY_LEFT KEY_4
#define KEY_RIGHT KEY_6

//功能键
#define KEY_BACKSPACE
#define KEY_ENTER
#define KEY_HOME


typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
typedef struct
{
	u8 Row0Val;
	u8 Row1Val;
	u8 Row2Val;
	u8 Row3Val;
	u8 Col0Val;
	u8 Col1Val;
	u8 Col2Val;
	u8 Col3Val;
}KeyBoradCode;

extern KeyBoradCode KBCode;

extern uint8_t KeyValue;
extern uint8_t KeyChange_Flag;
extern int Enter_flag;
char input_char(void);
int input_int(int row,int col);
double input_double(int row,int col);
double input_double_s(int row,int col);
double Char_2_Double(unsigned char *CHAR_temp);

u32 ReadKeyBoard(void);
int TestRow(void);
int TestCol(void);

void Init_KeyBoard(void);

void ROW_H_COL_L(void);
void ROW_L_COL_H(void);

void ENABLE_KeyBoard_CLK(void);
void ROW_IN_PULL_UP(void);
void ROW_OUT_L(void);
void COL_OUT_L(void);
void COL_IN_PULL_UP(void);

void InitRowColValue(u8 dir);

#endif
