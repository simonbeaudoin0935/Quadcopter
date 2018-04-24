################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/FreeRTOS/croutine.c \
../src/FreeRTOS/event_groups.c \
../src/FreeRTOS/heap_2.c \
../src/FreeRTOS/list.c \
../src/FreeRTOS/port.c \
../src/FreeRTOS/queue.c \
../src/FreeRTOS/stream_buffer.c \
../src/FreeRTOS/tasks.c \
../src/FreeRTOS/timers.c 

OBJS += \
./src/FreeRTOS/croutine.o \
./src/FreeRTOS/event_groups.o \
./src/FreeRTOS/heap_2.o \
./src/FreeRTOS/list.o \
./src/FreeRTOS/port.o \
./src/FreeRTOS/queue.o \
./src/FreeRTOS/stream_buffer.o \
./src/FreeRTOS/tasks.o \
./src/FreeRTOS/timers.o 

C_DEPS += \
./src/FreeRTOS/croutine.d \
./src/FreeRTOS/event_groups.d \
./src/FreeRTOS/heap_2.d \
./src/FreeRTOS/list.d \
./src/FreeRTOS/port.d \
./src/FreeRTOS/queue.d \
./src/FreeRTOS/stream_buffer.d \
./src/FreeRTOS/tasks.d \
./src/FreeRTOS/timers.d 


# Each subdirectory must supply rules for building sources it contributes
src/FreeRTOS/%.o: ../src/FreeRTOS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F407VGTx -DSTM32F407G_DISC1 -DDEBUG -DSTM32F40XX -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -I"/home/simon/Documents/Quadcopter/Firmware/StdPeriph_Driver/inc" -I"/home/simon/Documents/Quadcopter/Firmware/inc" -I"/home/simon/Documents/Quadcopter/Firmware/CMSIS/device" -I"/home/simon/Documents/Quadcopter/Firmware/CMSIS/core" -I"/home/simon/Documents/Quadcopter/Firmware/inc/FreeRTOS" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


