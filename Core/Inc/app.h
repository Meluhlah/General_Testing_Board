#ifndef INC_APP_H_
#define INC_APP_H_

#include "main.h"
#include "Addressable_leds.h"
#include "solenoid.h"
#include "servo.h"
#include "motor.h"
#include "string.h"
#include "fdcan.h"
#include "usart.h"
#include "i2c.h"


void app();
void ws2812b_set_single_color(uint8_t index, uint8_t red, uint8_t green, uint8_t blue);
void USB_CDC_RxHandler(uint8_t*, uint32_t);
void process_received_data();
void canbus_send_msg();
void rs485_send_msg();
void rs232_send_msg();


typedef enum{

    CMD_DC_MOTOR_SET_SPEED,
    CMD_DC_MOTOR_ROTATE_CW,
    CMD_DC_MOTOR_ROTATE_CCW,
    CMD_DC_MOTOR_STOP,
    CMD_SERVO_SET_PARAMS,
    CMD_SERVO_GO_TO_POSITION,
    CMD_LED_SET_COLOR_SINGLE,
    CMD_LED_SET_COLOR_ALL,
    CMD_LED_SET_INDEX,
    CMD_SOLENOID_ON,
	CMD_SOLENOID_ON_PEAK_HOLD,
    CMD_SOLENOID_OFF,
    CMD_SOLENOID_SET_HOLD_DUTY,
    CMD_CANBUS_SEND_MSG,
    CMD_RS485_SEND_MSG,
    CMD_RS232_SEND_MSG


} UartCommands_e;

typedef struct{
    uint32_t baudrate;
    uint8_t slave_addr;
    uint8_t data_length;
    uint8_t data[16];

}Rs485_t;

typedef struct{
    uint32_t baudrate;
    uint8_t data_length;
    uint8_t data[16];

}Rs232_t;

typedef struct{

    uint32_t Identifier;
    uint32_t IdType;
    uint32_t TxFrameType;
    uint32_t DataLength;
    uint8_t Data[8];

}Canbus_t;

typedef struct __attribute__((packed)){

    UartCommands_e  command;
    union{
        Canbus_t    canBus;
        Rs485_t     rs485;
        Rs232_t     rs232;
    }payload_u;

    uint16_t        data[4];
    uint32_t        checksum;

}   UartPacketRx_t;


#endif /* INC_APP_H_ */
