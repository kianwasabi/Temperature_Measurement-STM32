################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: 06_temperature_sensor.elf

dependents:
	-cd C:/Users/Besitzer/Documents/Studium/WiSe2020/01_Labor\ Datentechnik/Workspace/nucleo-l073rz_hal_lib/Debug && $(MAKE) all
	-cd C:/Users/Besitzer/Documents/Studium/WiSe2020/01_Labor\ Datentechnik/Workspace/lcd_shield_lib/Debug && $(MAKE) all

# Tool invocations
06_temperature_sensor.elf: $(OBJS) $(USER_OBJS) C:/Users/Besitzer/Documents/Studium/WiSe2020/01_Labor\ Datentechnik/Workspace/nucleo-l073rz_hal_lib/Debug/libnucleo-l073rz_hal_lib.a C:/Users/Besitzer/Documents/Studium/WiSe2020/01_Labor\ Datentechnik/Workspace/lcd_shield_lib/Debug/liblcd_shield_lib.a C:\Users\Besitzer\Documents\Studium\WiSe2020\01_Labor\ Datentechnik\Workspace\06_temperature_sensor/LinkerScript.ld
	@echo 'Building target: $@'
	@echo 'Invoking: MCU GCC Linker'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft -L"C:\Users\Besitzer\Documents\Studium\WiSe2020\01_Labor Datentechnik\Workspace\lcd_shield_lib\Debug" -T"C:/Users/Besitzer/Documents/Studium/WiSe2020/01_Labor Datentechnik/Workspace/06_temperature_sensor/LinkerScript.ld" -Wl,-Map=output.map -Wl,--gc-sections -o "06_temperature_sensor.elf" @"objects.list" $(USER_OBJS) $(LIBS) -lm
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

post-build:
	-@echo 'Generating binary and Printing size information:'
	arm-none-eabi-objcopy -O binary "06_temperature_sensor.elf" "06_temperature_sensor.bin"
	arm-none-eabi-size "06_temperature_sensor.elf"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build
C:/Users/Besitzer/Documents/Studium/WiSe2020/01_Labor\ Datentechnik/Workspace/nucleo-l073rz_hal_lib/Debug/libnucleo-l073rz_hal_lib.a:
C:/Users/Besitzer/Documents/Studium/WiSe2020/01_Labor\ Datentechnik/Workspace/lcd_shield_lib/Debug/liblcd_shield_lib.a:

-include ../makefile.targets
