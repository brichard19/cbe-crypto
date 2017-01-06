################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Custom includes added by CellDT makefile generator
-include import_spu.mk

# Macros added by the CDT's makefile generator

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
all: cbecrypto_ppu_test

# Tool invocations
cbecrypto_ppu_test: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: PPU GNU 64 bit C Linker'
	ppu-gcc -L"/home/user/workspace/cbecrypto_ppu/ppu-gnu64-debug" -L"/home/user/workspace/cell_ppu/ppu-gnu64-debug" -m64 -o"cbecrypto_ppu_test" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(C_DEPS)$(OBJS) cbecrypto_ppu_test
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
