#include "app.h"


Led_t ws2812b_led[AL_NUM_OF_LEDS] = {};
Solenoid_t solenoid = {};
Servo_t servo = {};
Motor_t motor = {};
UartPacketRx_t uartRxBuffer = {0};
FDCAN_TxHeaderTypeDef TxHeader;

void HAL_TIM_PWM_CompleteCallback(TIM_HandleTypeDef *htim){
	if(htim == &AL_TIM_HANDLER && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){

		HAL_TIM_PWM_Stop_DMA(htim, AL_TIM_CH);

	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	if(htim->Instance == TIM14)
		solenoid_check_peak_timeout(&solenoid, htim);

	if(htim->Instance == TIM7){	//TODO: Change to IO2_TIMER define

		HAL_TIM_Base_Stop_IT(htim);
		HAL_GPIO_WritePin(IO2_GPIO_Port, IO2_Pin, GPIO_PIN_RESET);

	}


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


void canbus_send_msg(){

	TxHeader.Identifier = uartRxBuffer.payload_u.canBus.Identifier;
	if(uartRxBuffer.payload_u.canBus.IdType == 0)
		TxHeader.IdType = FDCAN_STANDARD_ID;
	else
		TxHeader.IdType = FDCAN_EXTENDED_ID;

	if(uartRxBuffer.payload_u.canBus.TxFrameType == 0)
		TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	else
		TxHeader.TxFrameType = FDCAN_REMOTE_FRAME;

	TxHeader.DataLength = uartRxBuffer.payload_u.canBus.DataLength;

	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, uartRxBuffer.payload_u.canBus.Data) == HAL_OK) {
	}

}

void rs485_send_msg(){
	//huart2

	Rs485_t rs485 = uartRxBuffer.payload_u.rs485;

	if (rs485.baudrate > 0 && huart2.Init.BaudRate != rs485.baudrate) {
			huart2.Init.BaudRate = rs485.baudrate;
			HAL_UART_Init(&huart2);
	}

	// 2. Construct TX Frame [Slave Address (1 byte) + Data Payload]
		uint8_t txBuffer[17];
		uint8_t len = rs485.data_length;
		if (len > 16) len = 16; // Prevent buffer overrun

		txBuffer[0] = rs485.slave_addr;
		memcpy(&txBuffer[1], rs485.data, len);
		HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_SET);
		HAL_UART_Transmit(&huart2, txBuffer, len + 1, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET);

}


void rs232_send_msg(){
	//huart5
	Rs232_t rs232 = uartRxBuffer.payload_u.rs232;

		if (rs232.baudrate > 0 && huart5.Init.BaudRate != rs232.baudrate) {
			huart5.Init.BaudRate = rs232.baudrate;
			HAL_UART_Init(&huart5);
		}

		uint8_t len = rs232.data_length;
		if (len > 16) len = 16; // Prevent buffer overrun

		HAL_UART_Transmit(&huart5, rs232.data, len, HAL_MAX_DELAY);

}

void io2_pulse_ms(){

	__HAL_TIM_SET_AUTORELOAD(&IO2_TIMER, uartRxBuffer.data[0] -1);
	__HAL_TIM_SET_COUNTER(&IO2_TIMER,0);
	HAL_TIM_Base_Start_IT(&IO2_TIMER);
	HAL_GPIO_WritePin(IO2_GPIO_Port, IO2_Pin, GPIO_PIN_SET);

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
	case CMD_SERVO_SET_PARAMS:
		servo_set_params(&servo, uartRxBuffer.data[0], uartRxBuffer.data[1], uartRxBuffer.data[2]);
		break;

	case CMD_SERVO_GO_TO_POSITION:
		servo_drive(&servo, uartRxBuffer.data[0]);
		break;

	case CMD_SOLENOID_OFF:
		solenoid_off(&solenoid);
		break;

	case CMD_SOLENOID_ON:
		solenoid_on(&solenoid);
		break;

	case CMD_SOLENOID_ON_PEAK_HOLD:
		solenoid_on_peak_hold(&solenoid);
		break;

	case CMD_SOLENOID_SET_HOLD_DUTY:
		solenoid_set_hold_duty(&solenoid, uartRxBuffer.data[0]);
		break;

	case CMD_CANBUS_SEND_MSG:
		canbus_send_msg();
		break;

	case CMD_RS485_SEND_MSG:
		rs485_send_msg();
		break;

	case CMD_RS232_SEND_MSG:
		rs232_send_msg();
		break;

	case CMD_IO2_PULSE_MS:
		io2_pulse_ms();
		break;

	default:
		break;
	}
}



void app(){

	al_init(ws2812b_led);
	servo_init(&servo, &htim2, TIM_CHANNEL_1,270,500,2500);
	motor_init(&motor, &htim16, TIM_CHANNEL_1);
	solenoid_init(&solenoid, &htim14, TIM_CHANNEL_1, &htim17, 30);

	// CANBUS Init
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;       // Disable FD Bit Rate Switching
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;        // Classic CAN Mode
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;

	if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK) {
	    Error_Handler();
	}

	__HAL_TIM_SET_AUTORELOAD(&htim2, 99);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 65);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);


	while(1){
		static uint32_t blink_timer = 0;
		if(HAL_GetTick() - blink_timer > 1000)
		{
		  blink_timer = HAL_GetTick();
		  HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
		  // Push message to FIFO

//		  HAL_I2C_Master_Transmit(&hi2c2, 0x50, TxData, sizeof(TxData), 100);
//		  HAL_I2C_Master_Receive(&hi2c2, 0x50, TxData, sizeof(TxData), 100);


		}
//		  CDC_Transmit_FS(TxBuffer, TxBufferLen);
//		  HAL_Delay(100);


	}
}
