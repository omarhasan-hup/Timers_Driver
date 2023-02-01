################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL/LCD_DEBUG/lcd.c 

OBJS += \
./HAL/LCD_DEBUG/lcd.o 

C_DEPS += \
./HAL/LCD_DEBUG/lcd.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/LCD_DEBUG/%.o: ../HAL/LCD_DEBUG/%.c HAL/LCD_DEBUG/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"D:\eclipse work space\Timer_Driver\MCAL" -I"D:\eclipse work space\Timer_Driver\HAL\Timer_driver" -I"D:\eclipse work space\Timer_Driver\HAL\LCD_DEBUG" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


