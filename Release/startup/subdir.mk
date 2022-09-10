################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32l073xx.s 

OBJS += \
./startup/startup_stm32l073xx.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft -I"C:/Users/Besitzer/Documents/Studium/WiSe2020/01_Labor Datentechnik/Workspace/lcd_shield_lib" -I"C:/Users/Besitzer/Documents/Studium/WiSe2020/01_Labor Datentechnik/Workspace/lcd_shield_lib/include" -I"C:/Users/Besitzer/Documents/Studium/WiSe2020/01_Labor Datentechnik/Workspace/nucleo-l073rz_hal_lib/CMSIS/core" -I"C:/Users/Besitzer/Documents/Studium/WiSe2020/01_Labor Datentechnik/Workspace/nucleo-l073rz_hal_lib/CMSIS/device" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


