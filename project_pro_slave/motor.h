#include"def.h"

#ifndef __MOTOR_H__
#define __MOTOR_H__

void motor1_start();
void motor2_start();
void motor_start();

void motor1_stop();
void motor2_stop();
void motor_stop();

void motor1_init();
void motor2_init();

void forward(int count);
void forward_o(int count);
void forward_s(int count);
void turn_left(int count);
void turn_right(int count);
void overtake();

#endif