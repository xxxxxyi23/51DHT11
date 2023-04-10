#ifndef __SCREEN_H
#define __SCREEN_H

#include <reg52.h>

#define uchar unsigned char
#define uint  unsigned int

sbit EN=P2^5;		//1602ʹ������
sbit RW=P2^6;		//1602��д����	
sbit RS=P2^7;		//1602����/����ѡ������

void Delay();
void Write_Cmd(uchar );
void Write_Date(uchar );
void L1602_init(void);
void L1602_char(uchar ,uchar ,char );
void L1602_string(uchar ,uchar ,uchar *);
void L1602_int(uchar ,uchar ,int );

#endif