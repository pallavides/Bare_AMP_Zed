################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/adversary.c \
../src/main_production.c \
../src/pendulum_control.c \
../src/production_controller.c 

OBJS += \
./src/adversary.o \
./src/main_production.o \
./src/pendulum_control.o \
./src/production_controller.o 

C_DEPS += \
./src/adversary.d \
./src/main_production.d \
./src/pendulum_control.d \
./src/production_controller.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -Wall -O0 -g3 -I../../freertos_zynq_bsp_amp/ps7_cortexa9_1/include -I/home/controls/TAIGA/Rotary-Inverted-Pendulum/Rotary-Inverted-Pendulum.sdk/global_includes -c -fmessage-length=0 -MT"$@" -D__USE_AMP__ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


