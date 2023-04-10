#include <math.h>
#include "screen.h"

void Delay()
{
	int i,j;
	for(i=0;i<=10;i++)
	for(j=0;j<=2;j++);//��ʱԼ140us
}

//д1602����һ���ֽ�
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

//дһ���ֽ�����
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

//1602��ʼ��
void L1602_init(void)
{
	Write_Cmd(0x38);//16*2����ʾ
	Write_Cmd(0x0c);//����ʾ������ʾ���
	Write_Cmd(0x06);//��ַָ����λ����
	Write_Cmd(0x01);//����
	Write_Cmd(0xd0);//
}

//�ı�Һ��ĳλ��ֵ
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

//��ʾ���͵���ʪ�����ݣ���4λ��һλΪ����λ
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