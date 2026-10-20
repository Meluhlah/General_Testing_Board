/*
 * solenoid.c
 *
 *  Created on: Mar 31, 2026
 *      Author: Lidor
 */

#include "solenoid.h"


void solenoid_init(Solenoid_t* solenoid, TIM_HandleTypeDef* pwm_tim, uint32_t pwm_tim_channel, TIM_HandleTypeDef* peak_tim, uint16_t hold_duty)
{

	solenoid->pwm_tim = pwm_tim;
	solenoid->pwm_tim_channel = pwm_tim_channel;
	solenoid->peak_tim = peak_tim;
	solenoid->hold_duty = hold_duty;
	solenoid->state = SOLENOID_STATE_OFF;

}

void solenoid_check_peak_timeout(Solenoid_t* solenoid, TIM_HandleTypeDef *htim) {
    // Check if the timer that just finished is the one assigned to this solenoid
    if (htim->Instance == solenoid->peak_tim->Instance) {

        HAL_TIM_Base_Stop_IT(htim);
        // 1. Peak is over, drop to Hold Duty
        __HAL_TIM_SET_COMPARE(solenoid->pwm_tim, solenoid->pwm_tim_channel, solenoid->hold_duty);

        solenoid->state = SOLENOID_STATE_HOLD; // Mark as "Holding" or "Done"
    }
}

void solenoid_on(Solenoid_t* solenoid){

	__HAL_TIM_SET_COMPARE(solenoid->pwm_tim, solenoid->pwm_tim_channel, 100);
	HAL_TIM_PWM_Start(solenoid->pwm_tim, solenoid->pwm_tim_channel);
}

void solenoid_on_peak_hold(Solenoid_t* solenoid){

	__HAL_TIM_SET_COMPARE(solenoid->pwm_tim, solenoid->pwm_tim_channel, SOLENOID_PWM_PEAK_VALUE);
	HAL_TIM_PWM_Start(solenoid->pwm_tim, solenoid->pwm_tim_channel);
	solenoid->state = SOLENOID_STATE_PEAK;

	__HAL_TIM_SET_AUTORELOAD(solenoid->peak_tim, SOLENOID_PEAK_DURATION_MS-1);
	__HAL_TIM_SET_COUNTER(solenoid->peak_tim, 0);

	__HAL_TIM_CLEAR_IT(solenoid->peak_tim, TIM_IT_UPDATE);

	HAL_TIM_Base_Start_IT(solenoid->peak_tim);

}

void solenoid_off(Solenoid_t* solenoid){

	HAL_TIM_PWM_Stop(solenoid->pwm_tim, solenoid->pwm_tim_channel);
	solenoid->state = SOLENOID_STATE_OFF;

}

void solenoid_set_hold_duty(Solenoid_t* solenoid, uint16_t hold_duty)
{
	solenoid->hold_duty = hold_duty;
}
