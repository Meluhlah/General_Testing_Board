/*
 * servo.c
 *
 *  Created on: Apr 1, 2026
 *      Author: Lidor
 */

#include "servo.h"


void servo_init(Servo_t* servo, TIM_HandleTypeDef* pwm_tim, uint32_t pwm_tim_ch, uint16_t max_angle, uint16_t min_ticksUs, uint16_t max_ticksUs)
{
	servo->pwm_tim = pwm_tim;
	servo->pwm_tim_ch = pwm_tim_ch;
	servo->current_angle = 0;
	servo->target_angle = 0;
	servo->min_ticksUs = min_ticksUs;
	servo->max_ticksUs = max_ticksUs;
	servo->max_angle = max_angle;

}

void servo_set_params(Servo_t* servo, uint16_t minTicksUs, uint16_t maxTicksUs, uint16_t maxAngle)
{
	servo->max_angle = maxAngle;
	servo->min_ticksUs = minTicksUs;
	servo->max_ticksUs = maxTicksUs;

}


void servo_drive(Servo_t* servo, uint16_t targetAngle){

	if(targetAngle < 0)
		servo->target_angle = 0;

	if(targetAngle > servo->max_angle)
		servo->target_angle = servo->max_angle;

	// Formula: (Target / Max) * (Range) + offset

	float pulseUs = ((float)targetAngle / servo->max_angle) *
					(servo->max_ticksUs - servo->min_ticksUs) + servo->min_ticksUs;

	__HAL_TIM_SET_COMPARE(servo->pwm_tim, servo->pwm_tim_ch, (uint16_t)pulseUs);
	HAL_TIM_PWM_Start(servo->pwm_tim, servo->pwm_tim_ch);

}

void servo_sig_off(Servo_t* servo){

	HAL_TIM_PWM_Stop(servo->pwm_tim, servo->pwm_tim_ch);

}
