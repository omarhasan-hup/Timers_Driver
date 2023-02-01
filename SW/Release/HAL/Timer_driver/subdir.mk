################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL/Timer_driver/timer0.c 

OBJS += \
./HAL/Timer_driver/timer0.o 

C_DEPS += \
./HAL/Timer_driver/timer0.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/Timer_driver/%.o: ../HAL/Timer_driver/%.c HAL/Timer_driver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"D:\eclipse work space\Timer_Driver\MCAL" -I"D:\eclipse work space\Timer_Driver\HAL\Timer_driver" -I"D:\eclipse work space\Timer_Driver\HAL\LCD_DEBUG" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


