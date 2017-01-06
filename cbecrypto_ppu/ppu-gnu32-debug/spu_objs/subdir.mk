################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
SPUO_SRCS += \
../spu_objs/libcbecrypto_spu.spuo 

OBJS += \
./spu_objs/libcbecrypto_spu.o 


# Each subdirectory must supply rules for building sources it contributes
spu_objs/%.o: ../spu_objs/%.spuo
	@echo 'Building file: $<'
	@echo 'Invoking: PPU GNU 32 bit Embed SPU'
	ppu-embedspu -m32 $(subst .,_,$(basename $(notdir $@))) $< $@
	@echo 'Finished building: $<'
	@echo ' '


