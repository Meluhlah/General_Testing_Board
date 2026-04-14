/*
 * AL.h
 *
 *  Created on: Sep 6, 2025
 *      Author: Lidor
 */

#ifndef ADDRESSABLE_LEDS_ADDRESSABLE_LEDS_H_
#define ADDRESSABLE_LEDS_ADDRESSABLE_LEDS_H_

#include "tim.h"

/* Timer Configuration:
 * - SET ARR TO Calculated Value Below
 * - DMA: Direction - Memory to Peripheral
 *        Data Width - Memory(Byte) , Peripheral(Word)
 *
 *
 */

#define PROTOCOL_BIT_ORDER_RGB
//#define PROTOCOL_BIT_ORDER_RGBW

#ifdef PROTOCOL_BIT_ORDER_RGB
#define AL_TOTAL_DATA_BITS			(uint8_t)24
#define AL_HIGHEST_BIT_LOCATION		(uint8_t)23
#define AL_GREEN_DATA_SHIFT			(uint8_t)16
#define AL_RED_DATA_SHIFT			(uint8_t)8
#define AL_BLUE_DATA_SHIFT			(uint8_t)0
#endif

#ifdef PROTOCOL_BIT_ORDER_RGBW
#define AL_TOTAL_DATA_BITS			(uint8_t)32
#define AL_HIGHEST_BIT_LOCATION		(uint8_t)31
#define AL_GREEN_DATA_SHIFT			(uint8_t)24
#define AL_RED_DATA_SHIFT			(uint8_t)16
#define AL_BLUE_DATA_SHIFT			(uint8_t)8
#define AL_WHITE_DATA_SHIFT			(uint8_t)0
#endif

#define AL_TIMER_FREQ				(uint32_t)64000000
#define AL_REQUIRED_FREQ			(uint32_t)800000
#define AL_PERIOD					(uint32_t)(AL_TIMER_FREQ / AL_REQUIRED_FREQ)	// ARR = 80-1
#define AL_T1H_T0L					(uint16_t)(AL_PERIOD * 0.6)
#define AL_T1L_T0H					(uint16_t)(AL_PERIOD * 0.3)
#define AL_RESET_CYCLES				(uint16_t)100
#define AL_DATA_LENGTH				(uint16_t)(AL_TOTAL_DATA_BITS * AL_NUM_OF_LEDS + (AL_RESET_CYCLES))


#define AL_TIM_HANDLER				htim3
#define AL_TIM_CH					TIM_CHANNEL_1
#define AL_TIM_ARR					(80-1)

#define AL_NUM_OF_LEDS				(uint8_t)1

typedef struct{

	uint8_t red;
	uint8_t green;
	uint8_t blue;

#ifdef PROTOCOL_BIT_ORDER_RGBW
	uint8_t white;
#endif

}Led_t;

void al_init(Led_t* led);
void al_updateColors(Led_t* led);

#ifdef PROTOCOL_BIT_ORDER_RGB
void al_setSingleLed(Led_t* led, uint8_t index, uint8_t red, uint8_t green, uint8_t blue);
void al_setAllLeds(Led_t* led, uint8_t red, uint8_t green, uint8_t blue);
#endif

#ifdef PROTOCOL_BIT_ORDER_RGBW
void al_setSingleLed(Led_t* led, uint8_t index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
void al_setAllLeds(Led_t* led, uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
#endif

#endif /* ADDRESSABLE_LEDS_ADDRESSABLE_LEDS_H_ */
