/*
 * AL.c
 *
 *  Created on: Sep 6, 2025
 *      Author: Lidor
 */

#include "Addressable_leds.h"

volatile uint16_t pwm_dma_data[AL_DATA_LENGTH] = {0};


void al_init(Led_t* led){

	if(led != NULL)
		__HAL_TIM_SET_AUTORELOAD(&AL_TIM_HANDLER,AL_TIM_ARR);

	for(int i=0; i< AL_NUM_OF_LEDS; i++){

		led[i].red = 0;
		led[i].green = 0;
		led[i].blue = 0;

#ifdef PROTOCOL_BIT_ORDER_RGBW
		led[i].white = 0;
#endif

	}
	al_updateColors(led);

}

#ifdef PROTOCOL_BIT_ORDER_RGBW
void al_setSingleLed(Led_t* led, uint8_t index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white){

	if(index < AL_NUM_OF_LEDS){

		led[index].red = red;
		led[index].green = green;
		led[index].blue = blue;
		led[index].white = white;

	}

}

void al_setAllLeds(Led_t* led, uint8_t red, uint8_t green, uint8_t blue, uint8_t white){

	for(int i=0; i< AL_NUM_OF_LEDS; i++){

		led[i].red = red;
		led[i].green = green;
		led[i].blue = blue;
		led[i].white = white;

	}

}

#endif

#ifdef PROTOCOL_BIT_ORDER_RGB
void al_setAllLeds(Led_t* led, uint8_t red, uint8_t green, uint8_t blue){

	for(int i=0; i< AL_NUM_OF_LEDS; i++){

		led[i].red = red;
		led[i].green = green;
		led[i].blue = blue;

	}

}

void al_setSingleLed(Led_t* led, uint8_t index, uint8_t red, uint8_t green, uint8_t blue){

	if(index < AL_NUM_OF_LEDS){

		led[index].red = red;
		led[index].green = green;
		led[index].blue = blue;

	}

}


#endif

void al_updateColors(Led_t* led){

	uint32_t color = 0;
	uint16_t index = 0;

	for(int led_index = 0; led_index < AL_NUM_OF_LEDS; led_index++)
	{

#ifdef PROTOCOL_BIT_ORDER_RGB
		color = (led[led_index].green << 16)
				  | (led[led_index].red   << 8)
		      	  |  led[led_index].blue;

#elif defined PROTOCOL_BIT_ORDER_RGBW
		color = (led[led_index].green << 24)
				  | (led[led_index].red   << 16)
				  |  led[led_index].blue << 8
				  |  led[led_index].white;
#endif

		for(int bit_index=AL_TOTAL_DATA_BITS-1; bit_index>= 0; bit_index--){

			if(color & ((uint32_t)1 << bit_index)){

				pwm_dma_data[index] = AL_T1H_T0L;
			}

			else{

				pwm_dma_data[index] = AL_T1L_T0H;
				}
			++index;
		}
	}

	for(int i = index; i < AL_DATA_LENGTH;i++){

		pwm_dma_data[i] = 0;

	}

	HAL_TIM_PWM_Stop_DMA(&AL_TIM_HANDLER, AL_TIM_CH);

	HAL_TIM_PWM_Start_DMA(&AL_TIM_HANDLER, AL_TIM_CH, (uint32_t*)pwm_dma_data, AL_DATA_LENGTH);


}
