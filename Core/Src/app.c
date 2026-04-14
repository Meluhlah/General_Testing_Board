#include "app.h"


Led_t ws2812b_led[AL_NUM_OF_LEDS] = {};
Solenoid_t solenoid = {};
Servo_t servo = {};
Motor_t motor = {};

UartPacketRx_t uartRxBuffer = {0};


void HAL_TIM_PWM_CompleteCallback(TIM_HandleTypeDef *htim){
	if(htim == &AL_TIM_HANDLER && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){

		HAL_TIM_PWM_Stop_DMA(htim, AL_TIM_CH);

	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	solenoid_check_peak_timeout(&solenoid, htim);

}


void ws2812b_set_single_color(uint8_t index, uint8_t red, uint8_t green, uint8_t blue){

	al_setSingleLed(ws2812b_led, index, red, green, blue);
	al_updateColors(ws2812b_led);
}


void USB_CDC_RxHandler(uint8_t* Buf, uint32_t Len)
{

	if(Len >= sizeof(uartRxBuffer))
	{
		memcpy(&uartRxBuffer, Buf, sizeof(uartRxBuffer));
		process_received_data();
	}
}


void process_received_data(){

	switch(uartRxBuffer.command){

	case CMD_DC_MOTOR_ROTATE_CCW:
		motor_set_direction(&motor, MOTOR_DIRECTION_CCW);
		motor_set_speed(&motor, uartRxBuffer.data[0]);
		motor_en(&motor, 1);
		break;

	case CMD_DC_MOTOR_ROTATE_CW:
		motor_set_direction(&motor, MOTOR_DIRECTION_CW);
		motor_set_speed(&motor, uartRxBuffer.data[0]);
		motor_en(&motor, 1);
		break;

	case CMD_DC_MOTOR_STOP:
		motor_en(&motor, 0);
		break;

	case CMD_LED_SET_COLOR_SINGLE:
		if(uartRxBuffer.data[1] <= 255 && uartRxBuffer.data[2] <= 255 && uartRxBuffer.data[3] <= 255 && uartRxBuffer.data[4] <= 255)
		{
			al_setSingleLed(ws2812b_led, uartRxBuffer.data[0],uartRxBuffer.data[1], uartRxBuffer.data[2], uartRxBuffer.data[3]);
			al_updateColors(ws2812b_led);
		}

	}

}



void app(){


	al_init(ws2812b_led);
	servo_init(&servo, &htim2, TIM_CHANNEL_1);
	motor_init(&motor, &htim16, TIM_CHANNEL_1);
//	motor_set_speed(&motor, 100);
//	motor_set_direction(&motor, MOTOR_DIRECTION_CW);
//	motor_en(&motor, 1);
	servo_drive(&servo, 180);
	while(1){
		static uint32_t blink_timer = 0;
		if(HAL_GetTick() - blink_timer > 1000)
		{
		  blink_timer = HAL_GetTick();
		  HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
		}
//		  CDC_Transmit_FS(TxBuffer, TxBufferLen);
//		  HAL_Delay(100);


	}
}
