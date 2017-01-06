################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -fr

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: libcbecrypto_spu

# Tool invocations
libcbecrypto_spu: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: SPU GNU C Linker'
	spu-gcc -L"/home/user/workspace/cell_spu/spu-gnu-debug" -Wl,-N -o"libcbecrypto_spu" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(C_DEPS)$(OBJS) libcbecrypto_spu
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
