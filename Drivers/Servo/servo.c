/*
 * servo.c
 *
 *  Created on: Apr 1, 2026
 *      Author: Lidor
 */

#include "servo.h"


void servo_init(Servo_t* servo, TIM_HandleTypeDef* pwm_tim, uint32_t pwm_tim_ch){

	servo->pwm_tim = pwm_tim;
	servo->pwm_tim_ch = pwm_tim_ch;
	servo->current_deg = 0;
	servo->target_deg = 0;

}


void servo_drive(Servo_t* servo, int8_t degrees){

	if(degrees <= SERVO_MAX_DEGREES || degrees >= SERVO_MIN_DEGREES){

		uint8_t normalized_deg = degrees + 90;	// 0 <= degrees <= 180
		uint32_t result = ((normalized_deg * SERVO_MIN_TIM_TICKS) / 180) + SERVO_MIN_TIM_TICKS;
		__HAL_TIM_SET_COMPARE(servo->pwm_tim, servo->pwm_tim_ch, result);
		HAL_TIM_PWM_Start(servo->pwm_tim, servo->pwm_tim_ch);

	}

}

void servo_sig_off(Servo_t* servo){

	HAL_TIM_PWM_Stop(servo->pwm_tim, servo->pwm_tim_ch);

}
