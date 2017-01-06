################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main.c 

C_DEPS += \
./main.d 

OBJS += \
./main.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: PPU GNU 32 bit C Compiler with Debug Options'
	ppu-gcc -I"/home/user/workspace/cbecrypto_ppu/public/include" -O0 -g3 -Wall -c -fmessage-length=0 -Winline -Wextra -fno-inline -m32 -mabi=altivec -maltivec -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


