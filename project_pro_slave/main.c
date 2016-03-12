#include<iom128v.h>
#include"def.h"
#include"motor.h"

int cmp_count1=0,cmp_count2=0;			//步进电机脉冲计数
int cmp_count_temp=0;
uint circle_count=0;					//圈数计数
uint i=0;

uchar state=0;							//0--直行，1--计数直行，2--左转，3--右转

void main()
{ 
 motor_stop();
 port_init();
 motor1_init();
 motor2_init();
 eint0_fall_init();
 eint1_rise_init();
 eint2_rise_init();
 delayms(5000);
 SREG=0x80;
 
 while(circle_count<4)
 {
  state=0;
  
  DDRB|=(3<<0);
  PORTB&=~(3<<0);
  PORTB|=(1<<0);  
  motor1_start();
  
  while(!DOWN);
  EIMSK&=(~(1<<1))&(~(1<<2));
  delayms(800);
  //EIMSK|=(1<<1)|(1<<2);
  //EIFR|=(1<<1)|(1<<2);
  EIFR&=(~(1<<1))&(~(1<<2));
  eint1_rise_init();
  eint2_rise_init();
  
  for(i=0;i<2;i++)
  {
   EIMSK&=~(1<<0);
   while(!DOWN);	 	  					
   EIMSK&=(~(1<<1))&(~(1<<2));
   /*delayms(1500);
   eint1_rise_init();
   eint2_rise_init();*/
  
   state=1;
   
   forward(7600);
  
   state=2;
   
   EIMSK&=~(1<<0);						//转弯时关闭前方光电对管中断
   turn_left(2185);
   //EIFR|=(1<<0);
   EIFR&=(~(1<<0))&(~(1<<1))&(~(1<<2));
   eint0_fall_init();
   eint1_rise_init();
   eint2_rise_init();
   
   state=0;
   
   DDRB|=(3<<0);
   PORTB&=~(3<<0);
   PORTB|=(1<<0);
   motor1_start();
   
  }
  
  while(!DOWN);	 	
  EIMSK&=(~(1<<1))&(~(1<<2));
  /*delayms(1500);
  eint1_rise_init();
  eint2_rise_init();*/
  
  state=1;
    					  
  forward_s(7600); 
  
  state=2;
   
  EIMSK&=~(1<<0);						//转弯时关闭前方光电对管中断
  turn_left(2185);
  //EIFR|=(1<<0);
  EIFR&=(~(1<<0))&(~(1<<1))&(~(1<<2));
   eint0_fall_init();
   eint1_rise_init();
   eint2_rise_init();
  
  if(circle_count%2!=0)
  {
   state=1;
   forward(15200);
   
   EIMSK&=~(1<<0);
   DDRA&=~(1<<0);
   PORTA&=~(1<<0);
   while(FRONT);
   EIFR&=~(1<<0);
   delayms(5000);
   eint0_fall_init();
   
  }
  else
  {
   EIMSK&=~(1<<0);
   overtake();
   eint0_fall_init();
  }
  
  state=0;
  
  DDRB|=(3<<0);
  PORTB&=~(3<<0);
  PORTB|=(1<<0);
  motor1_start();
  while(!DOWN);
  EIMSK&=(~(1<<1))&(~(1<<2));
  /*delayms(1500);
  eint1_rise_init();
  eint2_rise_init();*/
  
  state=1;
  
  forward(8000);
  
  state=2;
  
  EIMSK&=~(1<<0);						//转弯时关闭前方光电对管中断
  turn_left(2100);
  //EIFR|=(1<<0);
  EIFR&=(~(1<<0))&(~(1<<1))&(~(1<<2));
   eint0_fall_init();
   eint1_rise_init();
   eint2_rise_init();
  
  //state=1;
  
  //forward(5300);
  
  circle_count++;
 }
 
  /*state=0;
  DDRB|=(3<<0);
  PORTB&=~(3<<0);
  PORTB|=(1<<0);  
  motor1_start();
  
  while(!DOWN);*/
  motor_stop();
  
  while(1);
}


#pragma interrupt_handler timer0_cmp_isr:16
void timer0_cmp_isr()
{
 cmp_count1--;
}

#pragma interrupt_handler timer2_cmp_isr:10
void timer2_cmp_isr()
{
 cmp_count2--;
}

#pragma interrupt_handler eint0_fall:2
void eint0_fall()
{
 motor1_stop();
 
 EIMSK&=~(1<<0);
 DDRD&=~(1<<0);
 PORTD&=~(1<<0);
 while(!(PIND&(1<<0)));
 motor1_start();
 eint0_fall_init();
 
}

#pragma interrupt_handler eint1_rise:3
void eint1_rise()
{
 cmp_count_temp=cmp_count1;
 
 motor_stop();
 
 DDRB|=(3<<0);			   //右转
 PORTB&=~(3<<0);
 TIMSK|=(1<<1)|(1<<7);
 motor_start();
 delayms(23);
 
 EIMSK&=~(1<<1);
 DDRD&=~(1<<1);
 PORTD&=~(1<<1);
 while(LEFT)
 {
  delayms(7);
 }
 motor_stop();
 
 DDRB|=(3<<0);			   //左转微调
 PORTB|=(3<<0);
 TIMSK|=(1<<1)|(1<<7);
 motor_start();
 delayms(20);
 motor_stop();
 
 eint1_rise_init();
 
 switch(state)
 {
  case 0:
  {
   DDRB|=(3<<0);
   PORTB&=~(3<<0);
   PORTB|=(1<<0);
   motor_start();
   break;
  }
  case 1:
  {
   cmp_count1=cmp_count_temp;
   DDRB|=(3<<0);
   PORTB&=~(3<<0);
   PORTB|=(1<<0);
   TIMSK|=(1<<1)|(1<<7);
   motor_start();
   break;
  }
  case 2:
  {
   cmp_count1=cmp_count_temp;
   DDRB|=(3<<0);
   PORTB|=(3<<0);
   TIMSK|=(1<<1)|(1<<7);
   motor_start();
   break;
  }
  case 3:
  {
   cmp_count1=cmp_count_temp;
   DDRB|=(3<<0);
   PORTB&=~(3<<0);
   TIMSK|=(1<<1)|(1<<7);
   motor_start();
   break;
  }
  default:
  break;
 }
}

#pragma interrupt_handler eint2_rise:4
void eint2_rise()
{
 cmp_count_temp=cmp_count1;
 
 motor_stop();
 
 DDRB|=(3<<0);			   //左转
 PORTB|=(3<<0);
 TIMSK|=(1<<1)|(1<<7);
 motor_start();
 delayms(23);
 
 EIMSK&=~(1<<2);
 DDRD&=~(1<<2);
 PORTD&=~(1<<2);
 while(RIGHT)
 {
  delayms(7);
 }
 motor_stop();
 
 DDRB|=(3<<0);			   //右转微调
 PORTB&=~(3<<0);
 TIMSK|=(1<<1)|(1<<7);
 motor_start();
 delayms(20);
 motor_stop();
 
 eint2_rise_init();
 
 switch(state)
 {
  case 0:
  {
   DDRB|=(3<<0);
   PORTB&=~(3<<0);
   PORTB|=(1<<0);
   motor1_start();
   break;
  }
  case 1:
  {
   cmp_count1=cmp_count_temp;
   DDRB|=(3<<0);
   PORTB&=~(3<<0);
   PORTB|=(1<<0);
   TIMSK|=(1<<1)|(1<<7);
   motor_start();
   break;
  }
  case 2:
  {
   cmp_count1=cmp_count_temp;
   DDRB|=(3<<0);
   PORTB|=(3<<0);
   TIMSK|=(1<<1)|(1<<7);
   motor_start();
   break;
  }
  case 3:
  {
   cmp_count1=cmp_count_temp;
   DDRB|=(3<<0);
   PORTB&=~(3<<0);
   TIMSK|=(1<<1)|(1<<7);
   motor_start();
   break;
  }
  default:
  break;
 }
 
}

void port_init()
{
 DDRA&=~(1<<0);
 PORTA&=~(1<<0);
}