################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Motor/motor.c 

OBJS += \
./Drivers/Motor/motor.o 

C_DEPS += \
./Drivers/Motor/motor.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Motor/%.o Drivers/Motor/%.su Drivers/Motor/%.cyclo: ../Drivers/Motor/%.c Drivers/Motor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B1xx -c -I../USB_Device/App -I../USB_Device/Target -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Lidor/STM32CubeIDE/workspace_1.19.0/General_Board/Drivers/Addressable_leds" -I"C:/Users/Lidor/STM32CubeIDE/workspace_1.19.0/General_Board/Drivers/Solenoid" -I"C:/Users/Lidor/STM32CubeIDE/workspace_1.19.0/General_Board/Drivers/Servo" -I"C:/Users/Lidor/STM32CubeIDE/workspace_1.19.0/General_Board/Drivers/Motor" -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-Motor

clean-Drivers-2f-Motor:
	-$(RM) ./Drivers/Motor/motor.cyclo ./Drivers/Motor/motor.d ./Drivers/Motor/motor.o ./Drivers/Motor/motor.su

.PHONY: clean-Drivers-2f-Motor

