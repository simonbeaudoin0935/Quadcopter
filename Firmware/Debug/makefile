################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include src/Tasks/subdir.mk
-include src/ParameterManager/subdir.mk
-include src/PMU/subdir.mk
-include src/Motor/subdir.mk
-include src/IMU/Madgwick/subdir.mk
-include src/IMU/MEMS/MPU9250/subdir.mk
-include src/IMU/MEMS/LSM9DS1/subdir.mk
-include src/IMU/MEMS/ADA9DOF/subdir.mk
-include src/FreeRTOS/subdir.mk
-include src/COM/UART/subdir.mk
-include src/COM/SWO/subdir.mk
-include src/COM/SPI/subdir.mk
-include src/COM/I2C/subdir.mk
-include src/subdir.mk
-include StdPeriph_Driver/src/subdir.mk
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
all: Firmware.elf

# Tool invocations
Firmware.elf: $(OBJS) $(USER_OBJS) /home/simon/Documents/Quadcopter/Firmware/LinkerScript.ld
	@echo 'Building target: $@'
	@echo 'Invoking: MCU GCC Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -u _printf_float -T"/home/simon/Documents/Quadcopter/Firmware/LinkerScript.ld" -Wl,-Map=output.map -Wl,--gc-sections -o "Firmware.elf" @"objects.list" $(USER_OBJS) $(LIBS) -lm
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

post-build:
	-@echo 'Generating binary and Printing size information:'
	arm-none-eabi-objcopy -O binary "Firmware.elf" "Firmware.bin"
	arm-none-eabi-size "Firmware.elf"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
