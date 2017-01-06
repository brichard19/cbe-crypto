################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../cbecrypto.c \
../sha1.c \
../specmd.c 

C_DEPS += \
./cbecrypto.d \
./sha1.d \
./specmd.d 

OBJS += \
./cbecrypto.o \
./sha1.o \
./specmd.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: PPU GNU 64 bit C Compiler with Debug Options'
	ppu-gcc -I"/home/user/workspace/cell_ppu/public/include" -I"/home/user/workspace/cbecrypto_ppu/public/include" -I"/home/user/workspace/cbecrypto_ppu/shared/include" -O0 -g3 -Wall -c -fmessage-length=0 -Winline -Wextra -fno-inline -m64 -mabi=altivec -maltivec -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


