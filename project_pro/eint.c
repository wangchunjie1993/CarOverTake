#include<iom128v.h>
#include"def.h"

void eint0_fall_init()  		//外部中断0，下降沿
{
 EICRA&=~(3<<0);
 EICRA|=(1<<1);
 EIMSK|=(1<<0);
}


void eint1_fall_init()		//外部中断1，下降沿
{
 EICRA&=~(3<<2);
 EICRA|=(1<<3);
 EIMSK|=(1<<1);
}

void eint1_rise_init()		//外部中断1，上升沿
{
 EICRA|=(3<<2);
 EIMSK|=(1<<1);
}

void eint2_fall_init()		//外部中断2，下降沿
{
 EICRA&=~(3<<4);
 EICRA|=(1<<5);
 EIMSK|=(1<<2);
}

void eint2_rise_init()		//外部中断2，上升沿
{
 EICRA|=(3<<4);
 EIMSK|=(1<<2);
}

void eint3_fall_init()		//外部中断3，下降沿
{
 EICRA&=~(3<<6);
 EICRA|=(1<<7);
 EIMSK|=(1<<3);
}


