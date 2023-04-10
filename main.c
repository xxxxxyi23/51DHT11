#include <reg52.h>
#include <math.h>
#include "screen.h"
#include "dht11.h"


sbit Led_1=P1^2;		//湿度报警灯
sbit Led_2=P1^3;		//温度报警灯
sbit key_1 = P3^6;  //取消报警信号
sbit key_2 = P3^5;	//取消报警亮灯
sbit Beep = P1^4;  //蜂鸣器

//定义标识
volatile bit FlagStartRH = 0;  //开始温湿度转换标志
volatile bit FlagKeyPress = 0; //有键按下


//定义温湿度传感器用外部变量
extern U8  U8FLAG,k;
extern U8  U8count,U8temp;
extern U8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
extern U8  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
extern U8  U8comdata;
extern U8  count, count_r;

U16 temp;
S16 temperature, humidity;
S16 idata TH, HH;  //温度上限和湿度上限
char * pSave;
U8 keyvalue, keyTH1, keyTH2, keyHH1, keyHH2;

//定义变量
U16 RHCounter;

bit led_state = 0;

uchar flag_en = 1;
uint xxx = 1;


//数据初始化
void Data_Init()
{
   RHCounter = 0;
   Led_1 = 1;
   Led_2 = 1;
   TH = 20;
   HH = 60;
   key_1 = 1;
   key_2 = 1;
}

//定时器0初始化
void Timer0_Init()
{
	ET0 = 1;        //允许定时器0中断
	TMOD = 1;       //定时器工作方式选择
	TL0 = 0x06;     
	TH0 = 0xf8;     //定时器赋予初值
	TR0 = 1;        //启动定时器
}

//定时器0中断
void Timer0_ISR (void) interrupt 1 using 0
{
	TL0 = 0x06;
	TH0 = 0xf8;     //定时器赋予初值

	//每2秒钟启动一次温湿度转换
    RHCounter ++;
    if (RHCounter >= 1000)
    {
       FlagStartRH = 1;
	   RHCounter = 0;
    }
}

//延时函数
void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
	for(y=114;y>0;y--);
}

void delay1()
{
	uint q,e;
	for(q=0;q<10000;q++)
	for(e=0;e<10000;e++);
}

void warning()
{
	static uchar value,value1,value2;
	if(temperature > TH)
	{			 
		value1 ++;		  //消除温度在边界时的干扰
		if(value1 > 2)
		{
			Led_1 = 0;		  //打开指示灯
		}		
	}
	else 
	{
		value1 = 0;
		Led_1 = 1;		  //关闭指示灯
	}

	if(humidity > HH)
	{			 
		value2 ++;		  //消除温度在边界时的干扰
		if(value2 > 2)
		{
			Led_2 = 0;		  //打开指示灯
		}		
	}
	else 
	{
		value2 = 0;
		Led_2 = 1;		  //关闭指示灯
	}

	if((Led_1 == 0) || (Led_2 == 0))
	{
		value ++;
		if(value >= 2)
			if(flag_en == 1)
			Beep = ~Beep; 	  //蜂鸣器报警	
			delay(100);
	}
	else 
	{
		Beep = 1;
		value = 0;
		flag_en = 1;
	}
}
/********************************************************************
* 名称 : Main()
* 功能 : 主函数
***********************************************************************/
void main()
{
    int i, j, testnum;
		int m, n, o;
    EA = 0;

	Timer0_Init();  //定时器0初始化

    Data_Init();  //数据初始化
	EA = 1;

	L1602_init();
	L1602_string(1,1," Welcome to T&H   ");
	L1602_string(2,1," Control System!  ");
	//延时
	for (i=0;i<500;i++)
	   for (j=0;j<1000;j++)
	   {;}
    //清屏
	L1602_string(1,1,"                ");
	L1602_string(2,1,"                ");
    L1602_string(1,1,"Tem:    C  TH:");
	L1602_string(2,1,"Hum:    %  HH:");
    
    //载入温度上限和湿度上限设定值
	L1602_char(1, 15, TH/10+48);
	L1602_char(1, 16, TH%10+48);
	L1602_char(2, 15, HH/10+48);
	L1602_char(2, 16, HH%10+48);
//	led_state = 0;

	while(1)
	{
       //温湿度转换标志检查
		 if (FlagStartRH == 1)
		 {
		     TR0 = 0;
             testnum = RH();
 			 FlagStartRH = 0;
			 TR0 = 1;
             //读出温湿度，只取整数部分
			 humidity = U8RH_data_H;
			 temperature = U8T_data_H;
 			 //显示温湿度
			 L1602_int(1,5,temperature);
		     L1602_int(2,5,humidity);
				warning();
//		if (temperature > TH) m = 1;
//		if (humidity > HH) m = 2;
//		if (temperature > TH && humidity > HH) m = 3;
//		switch(m)
//		{
//			case 1:Led_1 = 0;break;
//			case 2:Led_2 = 0;	break;
//			case 3:Led_1 = 0;Led_2 = 0;break;			
//		}
		 }
		 if (key_2 == 0)
		 {
			 delay(20);
			 if (key_2 == 0)
			 {
				Beep = 1;
				 break;
				 //return;
				 //delay1();
		     //delay(100000);				 
			 }
		 }
		 //delay(100000);
		//温湿度控制
//		if (temperature >= TH) m = 1;
//		if (humidity >= HH) m = 2;
//		if (temperature >= TH && humidity >= HH) m = 3;
//		switch(m)
//		{
//			case 1:Led_1 = 0;break;
//			case 2:Led_2 = 0;	break;
//			case 3:Led_1 = 0;Led_2 = 0;break;			
//		}
		
//		if (key_2 == 0)
//		{
//			delay(20);
//			if(key_2 == 0)
//			{
//				led_state = !led_state;
//			}
//			while(!key_2);
//		}
//		if (led_state){
//			Led_2 = 1;Led_1 = 1;
//		}else{
//			Led_2 = 0;Led_1 = 0;
//		}
	}
	delay1();
}
