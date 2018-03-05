################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include test/subdir.mk
-include src/preprocessors/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(BC_DEPS)),)
-include $(BC_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LLVM_EXECUTABLES += \
USER_OBJS.exe \


# All Target
all: TurboJS secondary-outputs

# Tool invocations
TurboJS: $(BCS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: LLVM Clang C++ linker'
	clang++ -L/usr/lib/gcc/x86_64-linux-gnu/4.9/ -o "TurboJS" $(BCS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

USER_OBJS.exe: TurboJS $(BCS) $(USER_OBJS)

# Other Targets
clean:
	-$(RM) $(BCS)$(CC_DEPS)$(C++_DEPS)$(LLVM_BC_EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(BC_DEPS)$(LLVM_EXECUTABLES)$(C_DEPS)$(CPP_DEPS) TurboJS
	-@echo ' '

secondary-outputs: $(LLVM_BC_EXECUTABLES)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
