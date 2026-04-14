/*
 * solenoid.h
 *
 *  Created on: Mar 31, 2026
 *      Author: Lidor
 */

#ifndef SOLENOID_SOLENOID_H_
#define SOLENOID_SOLENOID_H_

#include "tim.h"


/*	SOLENOID TIMER:
 * 		SET ARR TO 100:
 * 			ARR = 100 -> 100% DC
 * 			ARR = 50 -> 50% DC
 * 			......
 * 	DELAY TIMER:
 * 		SCALE TO 1MS STEPS
 * 		ENABLE INTERRUPT IN NVIC
 *
 */

#define SOLENOID_PWM_TIM				&htim14
#define SOLENOID_PWM_TIM_CH				TIM_CHANNEL_1
#define SOLENOID_PWM_PEAK_VALUE			100
#define SOLENOID_PWM_HOLD_VALUE			50

#define SOLENOID_PEAK_TIM				&htim17
#define SOLENOID_PEAK_DURATION_MS		1000


typedef enum{

	SOLENOID_STATE_OFF,
	SOLENOID_STATE_PEAK,
	SOLENOID_STATE_HOLD
}Solenoid_state_e;

typedef struct{

	TIM_HandleTypeDef *pwm_tim;
	TIM_HandleTypeDef *peak_tim;
	uint8_t hold_duty;
	Solenoid_state_e state;

}Solenoid_t;

void solenoid_init(Solenoid_t* solenoid);
void solenoid_off(Solenoid_t* solenoid);
void solenoid_on(Solenoid_t* solenoid);
void solenoid_on_peak_hold(Solenoid_t* solenoid);
void solenoid_check_peak_timeout(Solenoid_t* solenoid, TIM_HandleTypeDef *htim);

#endif /* SOLENOID_SOLENOID_H_ */
