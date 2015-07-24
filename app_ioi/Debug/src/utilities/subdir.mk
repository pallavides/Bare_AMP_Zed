################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utilities/axi_gpio.c \
../src/utilities/axi_spi.c \
../src/utilities/axi_uart.c \
../src/utilities/fifo_queue_wdt.c 

OBJS += \
./src/utilities/axi_gpio.o \
./src/utilities/axi_spi.o \
./src/utilities/axi_uart.o \
./src/utilities/fifo_queue_wdt.o 

C_DEPS += \
./src/utilities/axi_gpio.d \
./src/utilities/axi_spi.d \
./src/utilities/axi_uart.d \
./src/utilities/fifo_queue_wdt.d 


# Each subdirectory must supply rules for building sources it contributes
src/utilities/%.o: ../src/utilities/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -I../../app_ioi_bsp/IO_intermediary/include -I/home/controls/TAIGA-Pallavi/Rotary-Inverted-Pendulum-Zed/Rotary-Inverted-Pendulum-Zed.sdk/global_includes -c -fmessage-length=0 -MT"$@" -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mcpu=v9.5 -mxl-soft-mul -mhard-float -Wl,--no-relax -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


