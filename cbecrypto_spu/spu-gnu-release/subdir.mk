################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../aes.c \
../main.c \
../sha1.c 

C_DEPS += \
./aes.d \
./main.d \
./sha1.d 

OBJS += \
./aes.o \
./main.o \
./sha1.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SPU GNU C Compiler with Release Options'
	spu-gcc -I"/home/user/workspace/cbecrypto_ppu/shared/include" -I"/home/user/workspace/cell_spu" -O3 -Wall -c -fmessage-length=0 -Winline -Wextra -fno-inline -mtune=cell -mfloat=fast -mdouble=fast -Wno-main -march=cell -mea32 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


