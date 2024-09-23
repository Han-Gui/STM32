 #include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"
#include "led.h"
#include "adc.h"
#include "PE4302.H"
#include "Keyboard.h"
#include "ADF4351.h"
#include <math.h>
#include "spix.h"
#include "dac.h"
#include "595.h"
#include "TM1637.h"

/************************************************
频谱分析仪 - HAL库版本		
************************************************/

/************************************************
					修改日志

2023.6.30 by HanTJ
************************************************/

//Define
#define ADC_CH 8

//variables defination -->
double ADF4351VALUE;
extern int Enter_flag, Home_flag;
int menu_flag;
u8 touchandgive(void);
u16 vol_ori[2001];  //定义为全局变量，不然会爆栈
u16 vol_ori_disp[201];
float vol_dbm[2001];

//function defination -->
void LCD_Menu(unsigned int row_num, unsigned int col_num, char* ptr, char** menuitem);
void DEC2BCD(int dec);
void Show_BCD(int dec);

int main(void)
{
	//Variable Definitions:
	int adc1_test,adc2_test,adc3_test,dec1,dec2,dec3,dec4;
	char mode; //key_char;
	//u8 mark_i=0;//频标序号
	//u8 mark_enter_flag = 0;//mark界面enter检测
	int i=0,count=0,step=0,j=0,flag=0;
	u16 y1,y2,PE4302VALUE;//TLV5618VALUE;
	u16 DC_vol;
	uint32_t BCD,DEC;
	double PE4302dB;
	uint64_t Data;
	u8 data[64];
	int sum_num;	
	int cycle;
	//u16 vol_ori[201];
	//float vol_dbm[201];
	double freq_tmp = 0.0,scan_period = 0.0,freq_ave = 0.0;
	char* menuitem[6]={"Auto Scan","Manu Scan","Preset","Set DC","Attenuator","Spectrum"};
	float adc_base=0;
	float vol_max = -100, vol_max_fre = 0, mid_freq = 0;
	//频谱仪界面左侧信息初始化
	int list_xbase=2, list_ybase=75, list_ystep_1 = 30, list_ystep_2 = 20;
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2

  HAL_Init();                    	 			//初始化HAL库    
  Stm32_Clock_Init(336,8,2,7);   				//设置时钟,168Mhz
	delay_init(168);               	//初始化延时函数
	uart_init(115200);  	//初始化USART
	//reset_init();//初始化了PG1，没什么用
	//外设初始化
	LCD_Init();
	ADF4351Init();
	PE4302_Init();
	LED_Init();
	LCD_Display_Dir(1);	//设置LCD显示方向为1（横屏）
	LCD_DisplayOn();
	MY_ADC1_Init();		//初始化ADC
	MY_ADC2_Init();
	MY_ADC3_Init();
	Init_KeyBoard();	//初始化键盘
	SPIx_Init();
	DAC1_Init();               //初始化DAC1
	DAC2_Init();               //初始化DAC2
	TM1637_Init();
	
	//颜色宏定义
	#define BACKCOLOR BLACK
	#define SIGNALCOLOR YELLOW
	#define AXISCOLOR GRAY
	LCD_Clear(BACKCOLOR);
	//POINT_COLOR=RED;
	//ADF4351WriteFreq(40);
	//SPI1_Init();

	LCD_Set_Window(0,0,800,480);
	BACK_COLOR=BACKCOLOR;
	POINT_COLOR=GRAY;
	LCD_ShowxString(30,20,210,32,32,(u8*)"STM32F407",1);
	//LCD_ShowxString(30,60,272,32,32,(u8*)"80~100MHz Freq",1);
	POINT_COLOR=AXISCOLOR;
	menu_flag = 1;
	mode=1;
	
	#define STEP 0.1
	#define SPAN 20
	
	
	
	
//	while(1)
//	{
//				DEC2BCD(1);
//				DEC2BCD(2);
//				DEC2BCD(3);
//				DEC2BCD(4);
//				delay_ms(10);
//	}
	
	while(1)
	{
/*			测试用，输出实时ADC采样值			*/
		adc1_test = Get_Adc1_Average(ADC_CHANNEL_7, 5, 2);
		adc2_test = Get_Adc2_Average(ADC_CHANNEL_14, 5, 2);
		adc3_test = Get_Adc3_Average(ADC_CHANNEL_8, 5, 2);
//		POINT_COLOR = YELLOW;
		LCD_ShowxString(list_xbase, list_ybase, 12*4, 24, 24, "ADC:", 0);
		LCD_ShowxNum(list_xbase+48, list_ybase, adc1_test, 4, 24, 0);
		LCD_ShowxNum(list_xbase+48, list_ybase+40, adc2_test, 4, 24, 0);
		LCD_ShowxNum(list_xbase+48, list_ybase+80, adc3_test, 4, 24, 0);
/******************测试完毕*******************/
		
		if(menu_flag == 1)
		{
			LCD_Menu(3, 2, &mode, menuitem);
			Enter_flag=1;
			BACK_COLOR = BLACK;//
		}
		else
		{
			//LCD_ShowxString(350,30,450,32,32,(u8*)"Tips: Input\"B*\"to Enter Menu ",0);
			POINT_COLOR = SIGNALCOLOR;
			
			switch(mode)
			{
			case 1:									//Auto_Scanning Mode
			{
				if(Enter_flag==1&&!menu_flag)
				{					
					DEC = freq_tmp;
					dec1 = DEC / 1000;
					dec2 = (DEC / 100) % 10;
					dec3 = (DEC / 10) % 10;
					dec4 = DEC % 10;
					LCD_ShowxNum(list_xbase+48, list_ybase+80, dec1, 4, 24, 0);
					LCD_ShowxNum(list_xbase+48, list_ybase+110, dec2, 4, 24, 0);
					LCD_ShowxNum(list_xbase+48, list_ybase+140, dec3, 4, 24, 0);
					LCD_ShowxNum(list_xbase+48, list_ybase+170, dec4, 4, 24, 0);
					TM1637_showint(DEC);
					for(i=0;i<100;i++)
					{
						DEC2BCD(dec1);
						DEC2BCD(dec2);
						DEC2BCD(dec3);
						DEC2BCD(dec4);
					}

				}break;
			}
			case 2:									//Manual Scanning Mode
			{
				if(Enter_flag==1&&!menu_flag)
				{
					freq_tmp = 1.0 / freq_tmp;
					cycle = freq_tmp * 1000000;
					while(1)
					{
						DAC1_Set_Vol(3000);
						delay_us(1);
						DAC1_Set_Vol(0);
						delay_us(1);
					}
				}break;
			}
			case 3:										//Presetting Frequency Mode
			{
				if(Enter_flag==1&&!menu_flag)
				{
					while(1)
					{
						HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_SET);
						delay_us(1);
						HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_RESET);
						delay_us(1);
					}
					
				}break;
			}
			case 4:
			{
				if(Enter_flag==1&&!menu_flag)
				{
					LCD_Fill(0,100,800,480,BACKCOLOR);
					LCD_Fill(300,0,800,100,BACKCOLOR);
					LCD_ShowxString(30,100,272,32,32,(u8*)menuitem[mode-1],0);
					DC_vol = freq_tmp;
					if(DC_vol > 3300)
						DC_vol = 3300;
					DAC1_Set_Vol(DC_vol);
					LCD_ShowxFloat(30,200,DC_vol/1000.0+0.0005,3,10,32,0);
				}break;
			}
			case 5:
			{
				if(Enter_flag==1&&!menu_flag)
				{
					LCD_Fill(0,100,800,480,BACKCOLOR);
					LCD_Fill(300,0,800,100,BACKCOLOR);
					LCD_ShowxString(30,100,272,32,32,(u8*)menuitem[mode-1],0);
					LCD_ShowxString(30,180,272,32,32,(u8*)"Level",0);
					LCD_ShowxString(260,180,272,32,32,(u8*)"dB",0);
					//LCD_ShowxString(30,220,272,32,32,(u8*)"Output",0);
					step++;
					
					if(freq_tmp>63)freq_tmp=63;
					PE4302VALUE = freq_tmp;
					LCD_ShowxNum(120,180,PE4302VALUE,2,32,0);
					PE4302dB = PE4302VALUE / 2.0;
					LCD_ShowxFloat(180,180,PE4302dB+0.05,1,5,32,0);
					//LCD_ShowxFloat(140,220,Get_Adc3(ADC_CHANNEL_8)*3.3/4096,3,10,32,0);
					
					PE4302_Set(PE4302VALUE);

					if(step==12) step=0;
				}break;
			}
			case 6:
			{	
				if(Enter_flag==1&&!menu_flag)
				{
					LCD_ShowxString(30,220,272,32,32,(u8*)"6",0);

				}
				else//main函数的堆栈几乎满了，尽量不要在main里面增加变量，考虑复用
				{
					char key_char;

					while(1)
					{

						//阻塞式键盘输入
						key_char=input_char();
						switch(key_char){
							case KEY_LEFT:
							{
								LCD_ShowxString(30,220,272,32,32,(u8*)"LEFT",0);
								break;				
							}
							case KEY_RIGHT:
							{
								LCD_ShowxString(30,220,272,32,32,(u8*)"RIGHT",0);
								break;				
							}
							case KEY_ENTER:
							{
								menu_flag = 1;
								break;
							}
							case KEY_HOME:
							{
								Home_flag = 1;
								break;
							}
						}
						if(key_char == KEY_ENTER||key_char == KEY_HOME)
							break;

					}//按下enter或者home退出频标循环
				}	
				break;//case6的break
			}

			}
			
			//输入数字或指令
			freq_tmp = input_double_s(0,400);//键盘输入数据并显示在指定位置，非阻塞式，自带归零enter_flag
			if(menu_flag == 1)
			{//复用menu_flag为mark_enter_flag
				Enter_flag = 1;
				menu_flag = 0;//记得归零
			}
//			key_char=(char)freq_tmp;	//将double转换为整型
			if(Home_flag == 1) menu_flag=1;	//按下HOME键后返回主菜单
		}
	}	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


//菜单设置接口
#define BLOCK_COLOR BLACK			//块背景颜色
#define BLOCK_FONT_COLOR WHITE		//块字体颜色
#define BLOCK_FONT_SIZE 24			//块字体大小

#define SELECTED_BLOCK_COLOR BLACK	//选中块的背景颜色
#define SELECTED_FONT_COLOR RED		//选中块的字体颜色
#define SELECTED_FONT_SIZE 32		//选中块的字体大小
#define SMALL_BLOCK_COLOR RED	//选项前小方块的颜色

#define TITLE_COLOR YELLOW			//标题颜色
#define TITLE_FONT_SIZE 32			//标题字体大小
void LCD_Menu(unsigned int row_num, unsigned int col_num, char* mode, char** block_name)
{
	unsigned int block_sx, block_sy, block_ex, block_ey, block_width, block_height ;//s代表start，e代表end
	unsigned char mode_selected,mode_before;
	int i,j,i_selected,j_selected,i_before,j_before;//j colum   i row
	char key_char,count;
	int block_xbase;//模块起始x坐标
	int block_ybase;//模块起始y坐标
	
	mode_selected = mode_before = (*mode-1)%(row_num*col_num)+1;//*mode从1开始
	//设置背景颜色
	LCD_Clear(BACKCOLOR);
	BACK_COLOR=BACKCOLOR;
	POINT_COLOR=TITLE_COLOR;
	LCD_ShowString(0, 0, 272, TITLE_FONT_SIZE, TITLE_FONT_SIZE, "NJUEE 2023");
	//显示标题
	
	POINT_COLOR=TITLE_COLOR;
	LCD_ShowString(SSD_HOR_RESOLUTION/2-135, 30, 292, TITLE_FONT_SIZE, TITLE_FONT_SIZE, "Differential Probe");//右边3.25cm，左边2.75  原本
	
	LCD_ShowString(SSD_HOR_RESOLUTION/2+200, 80, 272, 24, 24, "By HanTJ,XDY,DC");
	//设置每个区块大小
	if(lcddev.dir==0){//vertical
		block_width = SSD_VER_RESOLUTION/col_num;
		block_height = SSD_HOR_RESOLUTION/row_num;
	}else{//horizontal	bingo宽度减半，高度30像素(字母高度)
		block_width = SSD_HOR_RESOLUTION/(1.5*col_num);
		block_height = 80;//SSD_VER_RESOLUTION/row_num;
	}
	block_xbase = +(SSD_HOR_RESOLUTION-2*block_width)/2;//模块起始x坐标
	block_ybase = (SSD_VER_RESOLUTION-3*block_height) * 0.75;//模块起始y坐标
	//区块颜色名字初始化
	for(i=0,count=0;i< col_num;i++){
		/*			 普通			*/
		for(j=0;j< row_num;j++){
			//设定坐标
			block_sx = block_xbase+i*block_width;
			block_sy = block_ybase+j*block_height;
			block_ex = block_sx + block_width-4;
			block_ey = block_sy + block_height-4;
			LCD_Fill(block_sx, block_sy, block_ex, block_ey, BLOCK_COLOR);
			//显示模块名称
			BACK_COLOR=BLOCK_COLOR;
			POINT_COLOR=BLOCK_FONT_COLOR;
			//LCD_ShowString(i*block_width+6,j*block_height+block_height/2,272,32,32BLOCK_FONT_SIZE,(u8*)block_name[count++]);
			LCD_ShowString(block_sx+block_width/2-100, block_sy+block_height/2-15, 272, BLOCK_FONT_SIZE, BLOCK_FONT_SIZE,(u8*)block_name[count++]);//15是字母高度一半
		}
	}
	/*			 选中			*/
	i_selected = (mode_selected-1)/row_num;//mode_selected取1-6,i_selected取0-2三个值; 
	j_selected = (mode_selected-1)%row_num;//j_selected取0-1两个值
	//设定坐标
	block_sx = block_xbase+i_selected*block_width;
	block_sy = block_ybase+j_selected*block_height;
	block_ex = block_sx + block_width-4;
	block_ey = block_sy + block_height-4;
	LCD_Fill(block_sx, block_sy, block_ex, block_ey, SELECTED_BLOCK_COLOR);//选中的块重设为蓝
	//画小块
	LCD_Fill(block_sx+block_width/2-100-20, block_sy+block_height/2-15+5, block_sx+block_width/2-100, block_sy+block_height/2-15+25, SMALL_BLOCK_COLOR);
	//显示选中名称
	BACK_COLOR =SELECTED_BLOCK_COLOR;
	POINT_COLOR =SELECTED_FONT_COLOR;
	LCD_ShowString(block_sx + block_width/2-100, block_sy + block_height/2-15, 272, SELECTED_FONT_SIZE, SELECTED_FONT_SIZE, (u8*)block_name[mode_selected-1]);
	
	while(1){
		//模式切换
		key_char=input_char();//键盘输入
		switch(key_char){
			case KEY_UP:{
				mode_selected -= 1;//位于下排
				if(!(mode_selected%row_num)){
					mode_selected = mode_selected+row_num;		}		
				break;
			}
			case KEY_DOWN:{
				if(!(mode_selected%row_num))
					mode_selected = mode_selected+1-row_num;
				else
					mode_selected += 1;
				break;				
			}
			case KEY_LEFT:{
				if(mode_selected>row_num)
					mode_selected -= row_num;
				else
					mode_selected += (col_num-1)*row_num;
				break;				
			}
			case KEY_RIGHT:{
				if(mode_selected<=(col_num-1)*row_num)
					mode_selected += row_num;
				else
					mode_selected -=(col_num-1)*row_num;
				break;				
			}
			case KEY_ENTER:break;
		}
		if(key_char == KEY_ENTER)break;
		//mode序号切换为行列号
		i_before = (mode_before-1)/row_num;
		j_before = (mode_before-1)%row_num;
		i_selected = (mode_selected-1)/row_num;
		j_selected = (mode_selected-1)%row_num;
		*mode=mode_selected;//返回选中的模块序号
		if(mode_before!=mode_selected){//如果发生了移动，重新设定颜色
			/*			 普通			*/
			block_sx = block_xbase+i_before*block_width;
			block_sy = block_ybase+j_before*block_height;
			block_ex = block_sx + block_width-4;
			block_ey = block_sy + block_height-4;
			LCD_Fill(block_sx, block_sy, block_ex, block_ey, BLOCK_COLOR);
			//显示模块名称
			BACK_COLOR=BLOCK_COLOR;
			POINT_COLOR=BLOCK_FONT_COLOR;
			LCD_ShowString(block_sx+block_width/2-100, block_sy+block_height/2-15, 272, BLOCK_FONT_SIZE, BLOCK_FONT_SIZE, (u8*)block_name[mode_before-1]);
			/*			 选中			*/
			block_sx = block_xbase+i_selected*block_width;
			block_sy = block_ybase+j_selected*block_height;
			block_ex = block_sx + block_width-4;
			block_ey = block_sy + block_height-4;
			LCD_Fill(block_sx, block_sy, block_ex, block_ey, SELECTED_BLOCK_COLOR);
			//画小块
			LCD_Fill(block_sx+block_width/2-100-20, block_sy+block_height/2-15+5, block_sx+block_width/2-100, block_sy+block_height/2-15+25, SMALL_BLOCK_COLOR);
			BACK_COLOR=SELECTED_BLOCK_COLOR;
			POINT_COLOR=SELECTED_FONT_COLOR;
			LCD_ShowString(block_sx+block_width/2-100, block_sy+block_height/2-15, 272, SELECTED_FONT_SIZE, SELECTED_FONT_SIZE, (u8*)block_name[mode_selected-1]);
		}
		mode_before = mode_selected;
	}
	menu_flag=0;
	Home_flag = 0;
	LCD_Clear(BACKCOLOR);
}

void DEC2BCD(int dec)  
{  
	switch(dec)
	{
		case 1:
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(2000);
			delay_ms(10);
			break;//0001
		case 2:
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(2000);
			delay_ms(10);
			DAC1_Set_Vol(0);
			delay_ms(10);
			break;//0010
		case 3:
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(2000);
			delay_ms(10);
			DAC1_Set_Vol(2000);
			delay_ms(10);
			break;//0011
		case 4:
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(2000);
			delay_ms(10);
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(0);
			delay_ms(10);
			break;//0100
		case 5:
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(2000);
			delay_ms(10);
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(2000);
			delay_ms(10);
			break;//0101
		case 6:
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(2000);
			delay_ms(10);
			DAC1_Set_Vol(2000);
			delay_ms(10);
			DAC1_Set_Vol(0);
			delay_ms(10);
			break;//0110
		case 7:
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(2000);
			delay_ms(10);
			DAC1_Set_Vol(2000);
			delay_ms(10);
			DAC1_Set_Vol(2000);
			delay_ms(10);
			break;//0111
		case 8:
			DAC1_Set_Vol(2000);
			delay_ms(10);
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(0);
			delay_ms(10);
			break;//1000
		case 9:
			DAC1_Set_Vol(2000);
			delay_ms(10);
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(2000);
			delay_ms(10);
			break;//1001
		default:
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(0);
			delay_ms(10);
			DAC1_Set_Vol(0);
			delay_ms(10);
			break;//0000
	}
} 


