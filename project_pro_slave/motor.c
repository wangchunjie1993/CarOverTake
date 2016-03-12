#include<iom128v.h>
#include"def.h"


extern int cmp_count1,cmp_count2;
extern uchar state;
void motor1_start()
{
 DDRB|=(1<<4);
}

void motor2_start()
{
 DDRB|=(1<<7);
}

void motor_start()
{
 DDRB|=(1<<4)|(1<<7);
}

void motor1_stop()
{
 DDRB&=~(1<<4);
}

void motor2_stop()
{
 DDRB&=~(1<<7);
}

void motor_stop()
{
 DDRB&=(~(1<<4))&(~(1<<7));
}

void motor1_init()
{
 TCCR0=0x00;
 TCNT0=0x00;
 OCR0=0x1c;
 TCCR0|=(7<<2);
}

void motor2_init()
{
 TCCR2=0x00;
 TCNT2=0x00;
 OCR2=0x1c;
 TCCR2|=0x1b;
}

void forward(int count)
{
 cmp_count1=count;
 DDRB|=(3<<0);
 PORTB&=~(3<<0);
 PORTB|=(1<<0);
 TIMSK|=(1<<1)|(1<<7);
 motor_start();
 if(count>2200)
 {
  while(cmp_count1>count-2000);
  //EIMSK|=(1<<1)|(1<<2);
  //EIFR|=(1<<1)|(1<<2);
  EIFR&=(~(1<<1))&(~(1<<2));
  eint1_rise_init();
  eint2_rise_init();
 }
 while(cmp_count1>0);
 motor_stop();
 TIMSK&=(~(1<<1))&(~(1<<7));
}

void forward_o(int count)
{
 cmp_count1=count;
 DDRB|=(3<<0);
 PORTB&=~(3<<0);
 PORTB|=(1<<0);
 TIMSK|=(1<<1)|(1<<7);
 motor_start();
 while(cmp_count1>0);
 motor_stop();
 TIMSK&=(~(1<<1))&(~(1<<7));
}

void forward_s(int count)
{
 cmp_count1=count;
 DDRB|=(3<<0);
 PORTB&=~(3<<0);
 PORTB|=(1<<0);
 TIMSK|=(1<<1)|(1<<7);
 motor_start();
 if(count>5500)
 {
  while(cmp_count1>count-5000);
  //EIMSK|=(1<<1)|(1<<2);
  //EIFR|=(1<<1)|(1<<2);
  EIFR&=(~(1<<1))&(~(1<<2));
  eint1_rise_init();
  eint2_rise_init();
 }
 while(cmp_count1>0);
 motor_stop();
 TIMSK&=(~(1<<1))&(~(1<<7));
}

void turn_left(int count)
{
 cmp_count1=count;
 DDRB|=(3<<0);
 PORTB|=(3<<0);
 TIMSK|=(1<<1)|(1<<7);
 motor_start();
 while(cmp_count1>0);
 motor_stop();
 TIMSK&=(~(1<<1))&(~(1<<7));
}

void turn_right(int count)
{
 cmp_count1=count;
 DDRB|=(3<<0);
 PORTB&=~(3<<0);
 TIMSK|=(1<<1)|(1<<7);
 motor_start();
 while(cmp_count1>0);
 motor_stop();
 TIMSK&=(~(1<<1))&(~(1<<7));
}

void overtake()
{
 //EIMSK&=(~(1<<0))&(~(1<<1))&(~(1<<2));

 state=1;
 forward_o(9200);
 
 state=2; 
 //EIMSK&=~(1<<0);						//转弯时关闭前方光电对管中断
 EIMSK&=(~(1<<0))&(~(1<<1))&(~(1<<2));
 turn_left(2185);

  
 state=1;
 forward_o(6800);
 //eint0_fall_init();
 //eint1_rise_init();
 //eint2_rise_init();
 
 state=3;
 //EIMSK&=~(1<<0);
 //EIMSK&=(~(1<<0))&(~(1<<1))&(~(1<<2));
 turn_right(2185);
 //EIFR|=(1<<0)|(1<<1)|(1<<2);
 EIFR&=(~(1<<0))&(~(1<<1))&(~(1<<2));
 //eint0_fall_init();
 eint1_rise_init();
 eint2_rise_init();
 
 state=1; 
 forward_o(12300);
 
 state=3;
 
 //EIMSK&=~(1<<0);
 EIMSK&=(~(1<<0))&(~(1<<1))&(~(1<<2));
 turn_right(2185);
 //eint0_fall_init();
 //eint1_rise_init();
 //eint2_rise_init();
 
 //EIMSK&=(~(1<<1))&(~(1<<2));
 
 state=1;
 
 forward_o(6800);
 
 state=2;
 
 //EIMSK&=~(1<<0);						//转弯时关闭前方光电对管中断
 //EIMSK&=(~(1<<0))&(~(1<<1))&(~(1<<2));
 turn_left(2185);
 //EIFR|=(1<<0)|(1<<1)|(1<<2);
 EIFR&=(~(1<<0))&(~(1<<1))&(~(1<<2));
 //eint0_fall_init(); 
 eint1_rise_init();
 eint2_rise_init();
 /*forward(4000);
 
 EIMSK&=~(1<<0);						//转弯时关闭前方光电对管中断
 turn_left(2180);
 eint0_fall_init();
 
 forward(5000);	  			//停到起始线*/
}

