/*
 * solenoid.c
 *
 *  Created on: Mar 31, 2026
 *      Author: Lidor
 */

#include "solenoid.h"


void solenoid_init(Solenoid_t* solenoid){

	solenoid->pwm_tim = SOLENOID_PWM_TIM;
	solenoid->peak_tim = SOLENOID_PEAK_TIM;
	solenoid->hold_duty = SOLENOID_PWM_HOLD_VALUE;
	solenoid->state = SOLENOID_STATE_OFF;

}

void solenoid_check_peak_timeout(Solenoid_t* solenoid, TIM_HandleTypeDef *htim) {
    // Check if the timer that just finished is the one assigned to this solenoid
    if (htim->Instance == solenoid->peak_tim->Instance) {

        // 1. Peak is over, drop to Hold Duty
        __HAL_TIM_SET_COMPARE(solenoid->pwm_tim, SOLENOID_PWM_TIM_CH, solenoid->hold_duty);

        // 2. Stop the timer (if not using hardware One-Pulse mode)
//        HAL_TIM_Base_Stop_IT(htim);

        solenoid->state = SOLENOID_STATE_HOLD; // Mark as "Holding" or "Done"
    }
}

void solenoid_on(Solenoid_t* solenoid){

	__HAL_TIM_SET_COMPARE(solenoid->pwm_tim, SOLENOID_PWM_TIM_CH, 100);
	HAL_TIM_PWM_Start(solenoid->pwm_tim, SOLENOID_PWM_TIM_CH);
}

void solenoid_on_peak_hold(Solenoid_t* solenoid){

	__HAL_TIM_SET_COMPARE(solenoid->pwm_tim, SOLENOID_PWM_TIM_CH, SOLENOID_PWM_PEAK_VALUE);
	HAL_TIM_PWM_Start(solenoid->pwm_tim, SOLENOID_PWM_TIM_CH);
	solenoid->state = SOLENOID_STATE_PEAK;
	__HAL_TIM_SET_AUTORELOAD(solenoid->peak_tim, SOLENOID_PEAK_DURATION_MS-1);
	__HAL_TIM_SET_COUNTER(solenoid->peak_tim, 0);
	__HAL_TIM_CLEAR_FLAG(solenoid->peak_tim, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start_IT(solenoid->peak_tim);

}

void solenoid_off(Solenoid_t* solenoid){

	HAL_TIM_PWM_Stop(solenoid->pwm_tim, SOLENOID_PWM_TIM_CH);
	solenoid->state = SOLENOID_STATE_OFF;

}
