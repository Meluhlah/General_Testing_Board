/*
 * h_bridge.h
 *
 *  Created on: Apr 2, 2026
 *      Author: Lidor
 */

#ifndef MOTOR_MOTOR_H_
#define MOTOR_MOTOR_H_

#include "tim.h"

#define MOTOR_FULL_DUTY				100

#define MOTOR_PWM_GPIO_PORT			MOT_PWM_GPIO_Port
#define MOTOR_PWM_PIN				MOT_PWM_Pin
#define MOTOR_DIRECTION_GPIO_PORT	MOT_DIR_GPIO_Port
#define MOTOR_DIRECTION_PIN			MOT_DIR_Pin

//TODO: Maybe Add direction define for flipped logic

typedef enum{

	MOTOR_DIRECTION_CW,
	MOTOR_DIRECTION_CCW

}Motor_direction_e;

typedef struct{

	TIM_HandleTypeDef* pwm_timer;
	uint32_t tim_ch;
	uint8_t current_duty;
	uint8_t motor_en;
	Motor_direction_e direction;

}Motor_t;

void motor_init				(Motor_t* motor, TIM_HandleTypeDef* pwm_timer, uint32_t tim_ch);
void motor_en				(Motor_t* motor, uint8_t motor_en);
void motor_set_direction	(Motor_t* motor, Motor_direction_e direction);
void motor_set_speed		(Motor_t* motor, uint8_t target_duty);



#endif /* MOTOR_MOTOR_H_ */
