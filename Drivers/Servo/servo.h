/*
 * servo.h
 *
 *  Created on: Apr 1, 2026
 *      Author: Lidor
 */

#ifndef SERVO_SERVO_H_
#define SERVO_SERVO_H_

#include "tim.h"


#define SERVO_MIN_DEGREES	(int8_t)(-90)
#define SERVO_MAX_DEGREES	(int8_t)(90)
#define SERVO_MIN_TIM_TICKS	1000
#define SERVO_MAX_TIM_TICKS	2000


typedef struct{

	TIM_HandleTypeDef* pwm_tim;
	uint32_t pwm_tim_ch;
	int8_t target_deg;
	int8_t current_deg;


}Servo_t;



void servo_init(Servo_t* servo, TIM_HandleTypeDef* pwm_tim, uint32_t pwm_tim_ch);
void servo_drive(Servo_t* servo, int8_t degrees);
void servo_sig_off(Servo_t* servo);


#endif /* SERVO_SERVO_H_ */
