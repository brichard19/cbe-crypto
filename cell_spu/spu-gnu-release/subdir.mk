################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libcell.c 

C_DEPS += \
./libcell.d 

OBJS += \
./libcell.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SPU GNU C Compiler with Release Options'
	spu-gcc -O3 -Wall -c -fmessage-length=0 -Winline -Wextra -fno-inline -mtune=cell -mfloat=fast -mdouble=fast -Wno-main -march=cell -mea32 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


