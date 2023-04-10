#include <math.h>
#include "screen.h"

void Delay()
{
	int i,j;
	for(i=0;i<=10;i++)
	for(j=0;j<=2;j++);//延时约140us
}

//写1602命令一个字节
void Write_Cmd(uchar cmd)
{
	RS = 0;
	RW = 0;
	P0 = cmd;
	EN = 1;
	Delay();
	EN = 0;
	Delay();
}

//写一个字节数据
void Write_Date(uchar date)
{
	RS = 1;
	RW = 0;
	P0 = date;
	EN = 1;
	Delay();
	EN = 0;
	Delay();	
}

//1602初始化
void L1602_init(void)
{
	Write_Cmd(0x38);//16*2的显示
	Write_Cmd(0x0c);//开显示，不显示光标
	Write_Cmd(0x06);//地址指针移位命令
	Write_Cmd(0x01);//清屏
	Write_Cmd(0xd0);//
}

//改变液晶某位的值
void L1602_char(uchar hang,uchar lie,char sign)
{
	uchar a;
	if(hang == 1) a = 0x80;
	if(hang == 2) a = 0xc0;
	a = a + lie - 1;
	Write_Cmd(a);
	Write_Date(sign);
}

//
void L1602_string(uchar hang,uchar lie,uchar *p)
{
	uchar a;
	if(hang == 1) a = 0x80;
	if(hang == 2) a = 0xc0;
	a = a + lie - 1;
	Write_Cmd(a);
	while(1)
	{
		if(*p == '\0') break;
		Write_Date(*p);
		p++;
	}
}

//显示整型的温湿度数据，共4位，一位为符号位
void L1602_int(uchar hang,uchar lie,int num)
{
	uint temp;
	uint gewei,shiwei,baiwei,sign;
	if(num > 0)
	{
		sign = 0;
	}
	else
	{
		sign = 1;
	}
	temp = abs(num);
	baiwei = temp/100;
	temp = temp-baiwei*100;
	shiwei = temp/10;
	gewei = temp-shiwei*10;
	num = abs(num);
	if (num >= 100)
	{
		if (sign = 0)
		{
			L1602_char(hang,lie,'-');
		}
		L1602_char(hang,lie+1,baiwei+'0');
		L1602_char(hang,lie+2,shiwei+'0');
		L1602_char(hang,lie+3,gewei+'0');
	}
	else if(num >= 10)
	{
		if (sign = 0)
		{
			L1602_char(hang,lie+1,'-');
		}
		L1602_char(hang,lie+2,shiwei+'0');
		L1602_char(hang,lie+3,gewei+'0');
	}
	else
	{
		if(sign = 0)
		{
			L1602_char(hang,lie+2,'-');
		}
		L1602_char(hang,lie+3,gewei+'0');
	}
}