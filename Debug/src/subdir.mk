################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/system_stm32l0xx.c 

OBJS += \
./src/main.o \
./src/system_stm32l0xx.o 

C_DEPS += \
./src/main.d \
./src/system_stm32l0xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft -std=c99 -DSTM32 -DSTM32L0 -DSTM32L073RZTx -DNUCLEO_L073RZ -DDEBUG -DSTM32L073xx -I"C:/Users/Besitzer/Documents/Studium/WiSe2020/01_Labor Datentechnik/Workspace/lcd_shield_lib" -I"C:/Users/Besitzer/Documents/Studium/WiSe2020/01_Labor Datentechnik/Workspace/lcd_shield_lib/include" -I"C:/Users/Besitzer/Documents/Studium/WiSe2020/01_Labor Datentechnik/Workspace/nucleo-l073rz_hal_lib/CMSIS/core" -I"C:/Users/Besitzer/Documents/Studium/WiSe2020/01_Labor Datentechnik/Workspace/nucleo-l073rz_hal_lib/CMSIS/device" -Og -g3 -pedantic -Wall -Wextra -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


