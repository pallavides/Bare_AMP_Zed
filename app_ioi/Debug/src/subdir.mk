################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/ioi_handler.c \
../src/main_ioi.c \
../src/pendulum_control.c \
../src/supervisory_communication.c \
../src/trigger_mechanisms.c 

OBJS += \
./src/ioi_handler.o \
./src/main_ioi.o \
./src/pendulum_control.o \
./src/supervisory_communication.o \
./src/trigger_mechanisms.o 

C_DEPS += \
./src/ioi_handler.d \
./src/main_ioi.d \
./src/pendulum_control.d \
./src/supervisory_communication.d \
./src/trigger_mechanisms.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -I../../app_ioi_bsp/IO_intermediary/include -I/home/controls/TAIGA-Pallavi/Rotary-Inverted-Pendulum-Zed/Rotary-Inverted-Pendulum-Zed.sdk/global_includes -c -fmessage-length=0 -MT"$@" -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mcpu=v9.5 -mxl-soft-mul -mhard-float -Wl,--no-relax -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


