#include "KeyBoard.h"
#include "sys.h"
#include "delay.h"
#include "lcd.h"

#define LCD12864 0
#define TFTLCD 1			// 1:On  0: Off
#define TFTFontSize 32


const char array_CHAR[16]= {'0','1','2','3','4','5','6','7','8','9','A','B','C','*','#','D'};
int Enter_flag;
KeyBoradCode KBCode;
uint8_t KeyValue;
uint8_t KeyChange_Flag=0;


double freq = 0;
//中断服务程序
//void EXTI9_5_IRQHandler(void)
//{
//	//HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);		//调用中断处理公用函数
//	delay_us(200);
//	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8) != RESET)//判断8中断是否发生
//	{
//		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8); //清除 LINE8 上的中断标志位
//	}
//	else;
//}

//接口函数
int input_int(int row,int col)
{
//	const char array_CHAR[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','*','#','D'};
  static int temp_int_s=0;
  int temp_int=0;
  char key_return_c=0;
  static int i;
//	int j=0;
  static unsigned char CHAR_temp[17];
  ROW_H_COL_L();
#if LCD12864
  LCD_TLine(row,col,(unsigned char*)"                ");//清除所要输入的LCD行
#endif
#if TFTLCD
  LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)"          ");
#endif
  while(1)
  {
    key_return_c=ReadKeyBoard();
    if(key_return_c<10)
    {
      CHAR_temp[i]=array_CHAR[key_return_c];
      CHAR_temp[i+1]='\0';
#if LCD12864
      LCD_TLine(row,col,CHAR_temp);
#endif
#if TFTLCD
      LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)CHAR_temp);
#endif
      temp_int_s=temp_int_s*10+key_return_c;
      i=i+1;
    }
    else if(key_return_c==13)//按下enter键
    {
      break;//直接跳出循环，待函数返回输入的值
    }
    else if(key_return_c==10)//按下A（退格）键
    {
      if(i>0)
      {
        i=i-1;
        CHAR_temp[i]='\0';
#if LCD12864
        LCD_TLine(row,col,(unsigned char*)"                ");//16个空格
        LCD_TLine(row,col,CHAR_temp);
#endif
#if TFTLCD
        LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)"           ");
        LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)CHAR_temp);
#endif
        temp_int_s=temp_int_s/10;
      }
    }
    else if(key_return_c==14)//按下‘#’（小数点）键，由于此函数为正数输入函数,故不作处理
    {
//			Decimal_point=i;
      CHAR_temp[i]='.';
      CHAR_temp[i+1]='\0';
#if LCD12864
      LCD_TLine(row,col,CHAR_temp);
#endif
#if TFTLCD
      LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)CHAR_temp);
#endif
      break;
    }
    //LCD_TData(row+1,col,(double)temp_int_s);
  }
  i=0;
  temp_int=temp_int_s;
  temp_int_s=0;
  return temp_int;
}

//接口函数；类似于阻塞性输入，该函数适合应用于有强烈时间先后顺序的地方
double input_double(int row,int col)
{
  double temp_double=0;
  char key_return_c=0;
  static int i;
  static unsigned char CHAR_temp[17];
  ROW_H_COL_L();
#if LCD12864
  LCD_TLine(row,col,(unsigned char*)"                ");//清除所要输入的LCD行
#endif
#if TFTLCD
  LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)"             ");
#endif
  while(1)
  {
    key_return_c=ReadKeyBoard();
    if(key_return_c<10)
    {
      CHAR_temp[i]=array_CHAR[key_return_c];
      CHAR_temp[i+1]='\0';
#if LCD12864
      LCD_TLine(row,col,CHAR_temp);
#endif
#if TFTLCD
      LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)CHAR_temp);
#endif
//			temp_int_s=temp_int_s*10+key_return_c;
      i=i+1;
    }
    else if(key_return_c==13)//按下enter键
    {
      break;//直接跳出循环，待函数返回输入的值
    }
    else if(key_return_c==10)//按下A（退格）键
    {
      if(i>0)
      {
        i=i-1;
        CHAR_temp[i]='\0';
#if LCD12864
        LCD_TLine(row,col,(unsigned char*)"                ");//16个空格
        LCD_TLine(row,col,CHAR_temp);
#endif
#if TFTLCD
        LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)"              ");
        LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)CHAR_temp);
#endif
      }
    }
    else if(key_return_c==14)//按下‘#’（小数点）键
    {
      CHAR_temp[i]='.';
      CHAR_temp[i+1]='\0';
#if LCD12864
      LCD_TLine(row,col,CHAR_temp);
#endif
#if TFTLCD
      LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)CHAR_temp);
#endif
      i=i+1;
    }
    else if(key_return_c==11)//'B'
    {

    }
    else if(key_return_c==13)//'C'
    {

    }
    else 										//'D';作为清屏键
    {
      return 0.0;
    }
  }
  temp_double=Char_2_Double(CHAR_temp);
  i=0;
  return temp_double;
}

//非阻塞性输入函数；接口函数
//本函数需要定义一个全局的return_flag
double input_double_s(int row,int col)
{
  double temp_double;
  char key_return_c;
  static int i;
  static unsigned char CHAR_temp[17];
  Enter_flag = 0;
  temp_double=0;
  key_return_c=0;
  ROW_H_COL_L();

  key_return_c=ReadKeyBoard();

//	key_return_c=KeyValue;
  if(key_return_c<10)
  {
    CHAR_temp[i]=array_CHAR[key_return_c];//array_CHAR[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','*','#','D'};
    CHAR_temp[i+1]='\0';
#if LCD12864
    LCD_TLine(row,col,(unsigned char*)"                ");
    LCD_TLine(row,col,CHAR_temp);
#endif
#if TFTLCD
    LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)"              ");
    LCD_ShowxString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)CHAR_temp,0);
#endif
    i=i+1;
  }
  else if(key_return_c==13)//按下enter键
  {
    Enter_flag = 1;
//		LCD_ShowxNum(300,200,Enter_flag,10,32,0);
  }
  else if(key_return_c==10)//按下A（退格）键
  {
    if(i>0)
    {
      i=i-1;
      CHAR_temp[i]='\0';
#if LCD12864
      LCD_TLine(row,col,(unsigned char*)"                ");//16个空格
      LCD_TLine(row,col,CHAR_temp);
#endif
#if TFTLCD
      LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)"              ");
      LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)CHAR_temp);
#endif
    }
  }
  else if(key_return_c==14)//按下‘#’（小数点）键
  {
    CHAR_temp[i]='.';
    CHAR_temp[i+1]='\0';
#if LCD12864
    LCD_TLine(row,col,(unsigned char*)"                ");
    LCD_TLine(row,col,CHAR_temp);
#endif
#if TFTLCD
    LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)"              ");
    LCD_ShowString(row,col,10*TFTFontSize,TFTFontSize,TFTFontSize,(u8*)CHAR_temp);
#endif
    i=i+1;
  }
  else if(key_return_c==11)//'B'
  {
    CHAR_temp[0]='1';
    CHAR_temp[1]='1';
    CHAR_temp[2]='\0';
  }
  else if(key_return_c==12)//'C'
  {
    CHAR_temp[0]='1';
    CHAR_temp[1]='2';
    CHAR_temp[2]='\0';
  }
  else if(key_return_c==15)//'D'
  {
    CHAR_temp[0]='1';
    CHAR_temp[1]='5';
    CHAR_temp[2]='\0';
  }

  if(Enter_flag==1)
  {
    i=0;
    temp_double=Char_2_Double(CHAR_temp);
    return temp_double;
  }
  else
  {
    temp_double=Char_2_Double(CHAR_temp);
//			LCD_ShowxNum(300,200,Enter_flag,10,32,0);
    return temp_double;
  }
}

//将一个数子数组转化为double类型数据；非接口函数
double Char_2_Double(unsigned char *CHAR_temp)
{
  double temp_double=0;
  long long int temp_=0;
  int i=0,j=0;
  for(j=0,temp_=0; j<17&&CHAR_temp[j]!='\0'; j=j+1)
  {
    if(CHAR_temp[j]!='.')
    {
      temp_=temp_*10+(CHAR_temp[j]-48);
    }
  }
  temp_double=temp_;
  i=0;
  while(CHAR_temp[i])
  {
    if(CHAR_temp[i]=='.')
    {
      i++;
      while(CHAR_temp[i])
      {
        temp_double=temp_double/10;
        i++;
      }
      break;
    }
    i++;
  }
  return temp_double;
}

u32 ReadKeyBoard(void)
{
  u32 return_temp=100;
  int nTemp=0;
  if(ROW0==0||ROW1==0||ROW2==0||ROW3==0)//B7，B6.。。。。
  {
    delay_us(500);
    if(ROW0==0||ROW1==0||ROW2==0||ROW3==0)
    {
      nTemp =TestRow();
      ROW_L_COL_H();
      ROW_L_COL_H();
      delay_us(100);
      nTemp|=TestCol();
      switch(nTemp)
      {
      case 126:
        return_temp= 10;
        break;//'1'
      case 190:
        return_temp= 11;
        break;//'2'
      case 222:
        return_temp= 12;
        break;//'3'
      case 238:
        return_temp= 15;
        break;//'mode'
      case 125:
        return_temp= 3;
        break;//'4'
      case 189:
        return_temp= 6;
        break;//'5'
      case 221:
        return_temp= 9;
        break;//'6'
      case 237:
        return_temp= 14;
        break;//'B'
      case 123:
        return_temp= 2;
        break;//'7'
      case 187:
        return_temp= 5;
        break;//'8'
      case 219:
        return_temp= 8;
        break;//'9'
      case 235:
        return_temp= 0;
        break;//'C'
      case 119:
        return_temp= 1;
        break;//'*'
      case 183:
        return_temp= 4;
        break;//'0'
      case 215:
        return_temp= 7;
        break;//'#'
      case 231:
        return_temp= 13;
        break;//'D'
      default:
        return_temp= 160;
        break;
      }
      while(COL0==0||COL1==0||COL2==0||COL3==0);
      delay_us(5);
      while(COL0==0||COL1==0||COL2==0||COL3==0);
    }
  }
  ROW_H_COL_L();
  return return_temp;
}

int TestRow(void)
{
  int nTemp=0;
  if(ROW0==0)
    nTemp=KBCode.Row0Val;		//0x70;
  else if(ROW1==0)
    nTemp=KBCode.Row1Val;		//0xb0;
  else if(ROW2==0)
    nTemp=KBCode.Row2Val;		//0xd0;
  else if(ROW3==0)
    nTemp=KBCode.Row3Val;		//0xe0;
  else
    nTemp=0x0f;		//0xf0;

  return nTemp;
}

int TestCol(void)
{
  int nTemp=0;
  if(COL0==0)
    nTemp=KBCode.Col0Val;		//0x07;

  else if(COL1==0)
    nTemp=KBCode.Col1Val;		//0x0b;
  else if(COL2==0)
    nTemp=KBCode.Col2Val;		//0x0d;
  else if(COL3==0)
    nTemp=KBCode.Col3Val;		//0x0e;
  else
    nTemp=0x00;
  return nTemp;
}



void Init_KeyBoard(void)//更名，整合，在main里调用的时候注意
{
	//键盘端口初始化Init_KeyBoard_Port，把KEY_INTERRUPT_GPIO_BIT还原回GPIO_Pin_8
	GPIO_InitTypeDef GPIO_Initure;
	InitRowColValue(1);
	ENABLE_KeyBoard_CLK();
	__HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOB时钟
	
	GPIO_Initure.Pin=GPIO_PIN_8;            //PB8初始化
//	GPIO_Initure.Mode=GPIO_MODE_INPUT;		//输入
//	GPIO_Initure.Speed=GPIO_SPEED_HIGH; 	//高速
//	GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
	
	//初始化中断优先级KeyBoard_Interrupt_Init
	GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //上升沿触发
	GPIO_Initure.Pull=GPIO_PULLUP;				//此处有争议
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	
	HAL_NVIC_SetPriority(EXTI9_5_IRQn,2,2); //抢占优先级2，子优先级2      //抢占优先级为2，子优先级为2
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);       //使能中断线8（包含在9_5中）

//  KeyBoard_Interrupt_Init();
	ROW_H_COL_L();
}

void ROW_H_COL_L(void)
{
  ROW_IN_PULL_UP();
  COL_OUT_L();
}

void ROW_L_COL_H()
{
  COL_IN_PULL_UP();
  ROW_OUT_L();
}

void ENABLE_KeyBoard_CLK(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOA时钟
	__HAL_RCC_GPIOG_CLK_ENABLE();           //开启GPIOC时钟
}

void ROW_IN_PULL_UP(void)
{
  int i=0;
  GPIO_InitTypeDef ROW_GPIO[4];
  //init row
  for(i=0; i<4; i++)
  {
	ROW_GPIO[i].Mode=GPIO_MODE_INPUT; //输入
	ROW_GPIO[i].Speed=GPIO_SPEED_HIGH; //高速  
	ROW_GPIO[i].Pull=GPIO_PULLUP; //上拉
  }
	ROW_GPIO[0].Pin=ROW0_GPIO_BIT;
	ROW_GPIO[1].Pin=ROW1_GPIO_BIT;
	ROW_GPIO[2].Pin=ROW2_GPIO_BIT;
	ROW_GPIO[3].Pin=ROW3_GPIO_BIT;

	HAL_GPIO_Init(ROW0_GPIO, &ROW_GPIO[0]);
	HAL_GPIO_Init(ROW1_GPIO, &ROW_GPIO[1]);
	HAL_GPIO_Init(ROW2_GPIO, &ROW_GPIO[2]);
	HAL_GPIO_Init(ROW3_GPIO, &ROW_GPIO[3]);
}

void ROW_OUT_L(void)
{
  int i=0;
  GPIO_InitTypeDef ROW_GPIO[4];
  //init row
  for(i=0; i<4; i++)
  {
	  ROW_GPIO[i].Mode=GPIO_MODE_OUTPUT_PP; //推挽输出
	  ROW_GPIO[i].Speed=GPIO_SPEED_HIGH; //高速(100M)  
	  ROW_GPIO[i].Pull=GPIO_PULLDOWN; //下拉
  }
	ROW_GPIO[0].Pin=ROW0_GPIO_BIT;
	ROW_GPIO[1].Pin=ROW1_GPIO_BIT;
	ROW_GPIO[2].Pin=ROW2_GPIO_BIT;
	ROW_GPIO[3].Pin=ROW3_GPIO_BIT;

	HAL_GPIO_Init(ROW0_GPIO, &ROW_GPIO[0]);
	HAL_GPIO_Init(ROW1_GPIO, &ROW_GPIO[1]);
	HAL_GPIO_Init(ROW2_GPIO, &ROW_GPIO[2]);
	HAL_GPIO_Init(ROW3_GPIO, &ROW_GPIO[3]);
  ROW_L;
}


void COL_OUT_L(void)
{
  int i=0;
  GPIO_InitTypeDef COL_GPIO[4];
  //init col
  for(i=0; i<4; i++)
  {
	  COL_GPIO[i].Mode=GPIO_MODE_OUTPUT_PP; //推挽输出
	  COL_GPIO[i].Speed=GPIO_SPEED_HIGH; //高速(100M)  
	  COL_GPIO[i].Pull=GPIO_PULLDOWN; //下拉
  }
  COL_GPIO[0].Pin=COL0_GPIO_BIT;
  COL_GPIO[1].Pin=COL1_GPIO_BIT;
  COL_GPIO[2].Pin=COL2_GPIO_BIT;
  COL_GPIO[3].Pin=COL3_GPIO_BIT;

  	HAL_GPIO_Init(COL0_GPIO, &COL_GPIO[0]);
	HAL_GPIO_Init(COL1_GPIO, &COL_GPIO[1]);
	HAL_GPIO_Init(COL2_GPIO, &COL_GPIO[2]);
	HAL_GPIO_Init(COL3_GPIO, &COL_GPIO[3]);
  COL_L;
}


void COL_IN_PULL_UP(void)
{
  int i=0;
  GPIO_InitTypeDef COL_GPIO[4];
  //init col
  for(i=0; i<4; i++)
  {
	COL_GPIO[i].Mode=GPIO_MODE_INPUT; //输入
	COL_GPIO[i].Speed=GPIO_SPEED_HIGH; //高速100M  
	COL_GPIO[i].Pull=GPIO_PULLUP; //上拉
  }
  COL_GPIO[0].Pin=COL0_GPIO_BIT;
  COL_GPIO[1].Pin=COL1_GPIO_BIT;
  COL_GPIO[2].Pin=COL2_GPIO_BIT;
  COL_GPIO[3].Pin=COL3_GPIO_BIT;

	HAL_GPIO_Init(COL0_GPIO, &COL_GPIO[0]);
	HAL_GPIO_Init(COL1_GPIO, &COL_GPIO[1]);
	HAL_GPIO_Init(COL2_GPIO, &COL_GPIO[2]);
	HAL_GPIO_Init(COL3_GPIO, &COL_GPIO[3]);
}

void InitRowColValue(uint8_t dir)
{
  if(dir==KEYBOARD_MIRROR)	//MIRROR：
  {
    KBCode.Col0Val=0x70;	
    KBCode.Col1Val=0xb0;	
    KBCode.Col2Val=0xd0;	
    KBCode.Col3Val=0xe0;	
    KBCode.Row0Val=0x07;	//0111
    KBCode.Row1Val=0x0b;	//1011
    KBCode.Row2Val=0x0d;	//1101
    KBCode.Row3Val=0x0e;	//1110
  }
  else if(dir==KEYBOARD_REAL)
  {
    KBCode.Col0Val=0x07;
    KBCode.Col1Val=0x0b;
    KBCode.Col2Val=0x0d;
    KBCode.Col3Val=0x0e;
    KBCode.Row0Val=0x70;
    KBCode.Row1Val=0xb0;
    KBCode.Row2Val=0xd0;
    KBCode.Row3Val=0xe0;
  }
}


char input_char(void){
	char key_value;
	key_value = ReadKeyBoard();
	return array_CHAR[key_value];
}

