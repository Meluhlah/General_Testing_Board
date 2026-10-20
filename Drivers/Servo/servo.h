/*
 * servo.h
 *
 *  Created on: Apr 1, 2026
 *      Author: Lidor
 */

#ifndef SERVO_SERVO_H_
#define SERVO_SERVO_H_

#include "tim.h"

typedef struct{

	TIM_HandleTypeDef* pwm_tim;
	uint32_t pwm_tim_ch;
	uint16_t target_angle;
	uint16_t current_angle;	// if feedback is available
	uint16_t max_angle;
	uint16_t min_ticksUs;
	uint16_t max_ticksUs;

}Servo_t;



void servo_init(Servo_t* servo, TIM_HandleTypeDef* pwm_tim, uint32_t pwm_tim_ch, uint16_t max_angle, uint16_t min_ticksUs, uint16_t max_ticksUs);
void servo_set_params(Servo_t* servo, uint16_t minTicksUs, uint16_t maxTicksUs, uint16_t maxAngle);
void servo_drive(Servo_t* servo, uint16_t targetAngle);
void servo_sig_off(Servo_t* servo);


#endif /* SERVO_SERVO_H_ */
