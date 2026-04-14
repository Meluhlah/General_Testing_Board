#ifndef INC_APP_H_
#define INC_APP_H_

#include "main.h"
#include "Addressable_leds.h"
#include "solenoid.h"
#include "servo.h"
#include "motor.h"
#include "string.h"


void app();
void ws2812b_set_single_color(uint8_t index, uint8_t red, uint8_t green, uint8_t blue);
void USB_CDC_RxHandler(uint8_t*, uint32_t);
void process_received_data();


typedef enum{

    CMD_DC_MOTOR_SET_SPEED,
    CMD_DC_MOTOR_ROTATE_CW,
    CMD_DC_MOTOR_ROTATE_CCW,
	CMD_DC_MOTOR_STOP,
    CMD_SERVO_MOTOR_MINUS_90_DEG,
    CMD_SERVO_MOTOR_PLUS_90_DEG,
    CMD_SERVO_MOTOR_0_DEG,
    CMD_SERVO_MOTOR_DIAL_DEG,
    CMD_LED_SET_COLOR_SINGLE,
    CMD_LED_SET_COLOR_ALL,
    CMD_LED_SET_INDEX,


} UartCommands_e;


typedef struct __attribute__((packed)){

    UartCommands_e  command;
    uint16_t        data[4];
    uint32_t        checksum;

}   UartPacketRx_t;


#endif /* INC_APP_H_ */
