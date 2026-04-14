/*
 * h_bridge.c
 *
 *  Created on: Apr 2, 2026
 *      Author: Lidor
 */

#include "motor.h"

void motor_init		(Motor_t* motor, TIM_HandleTypeDef* pwm_timer, uint32_t tim_ch){

	if(motor == NULL || pwm_timer == NULL)
		return;

	motor->pwm_timer = pwm_timer;
	motor->tim_ch = tim_ch;
	__HAL_TIM_SET_AUTORELOAD(motor->pwm_timer, MOTOR_FULL_DUTY);
	__HAL_TIM_SET_COMPARE(motor->pwm_timer, motor->tim_ch, 0);
	motor->current_duty = 0;
	motor->direction = MOTOR_DIRECTION_CW;
	motor->motor_en = 0;
	HAL_TIM_PWM_Stop(pwm_timer, tim_ch);
	HAL_GPIO_WritePin(MOTOR_DIRECTION_GPIO_PORT, MOTOR_DIRECTION_PIN, GPIO_PIN_SET);


}


void motor_en		(Motor_t* motor, uint8_t motor_en){

	if(motor_en == 1)
	{
		HAL_TIM_PWM_Start(motor->pwm_timer, motor->tim_ch);
		motor->motor_en = 1;
	}

	else
	{
		HAL_TIM_PWM_Stop(motor->pwm_timer, motor->tim_ch);
		motor->motor_en = 0;
	}

}


void motor_set_direction	(Motor_t* motor, Motor_direction_e direction){

	motor->direction = direction;
	HAL_GPIO_WritePin(MOTOR_DIRECTION_GPIO_PORT, MOTOR_DIRECTION_PIN, direction);
}


void motor_set_speed(Motor_t* motor, uint8_t target_duty){

	if(target_duty <= (uint8_t)__HAL_TIM_GET_AUTORELOAD(motor->pwm_timer))
	{
		__HAL_TIM_SET_COMPARE(motor->pwm_timer, motor->tim_ch, target_duty);
		motor->current_duty = target_duty;
	}

}
