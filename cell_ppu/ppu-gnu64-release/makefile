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
all: libcell_ppu.a

# Tool invocations
libcell_ppu.a: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: PPU GNU Archiver'
	ppu-ar -qcs "libcell_ppu.a" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ARCHIVES)$(C_DEPS)$(OBJS) libcell_ppu.a
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
