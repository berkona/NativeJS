################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tool/GenerateAST.cpp 

OBJS += \
./tool/GenerateAST.o 

CPP_DEPS += \
./tool/GenerateAST.d 


# Each subdirectory must supply rules for building sources it contributes
tool/%.o: ../tool/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/jon/src/PL-stuff/TurboJS/lib/cute" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


