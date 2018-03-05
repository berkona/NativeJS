################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/preprocessors/Aggregator.cpp \
../src/preprocessors/Preprocessor.cpp \
../src/preprocessors/Resolver.cpp 

OBJS += \
./src/preprocessors/Aggregator.o \
./src/preprocessors/Preprocessor.o \
./src/preprocessors/Resolver.o 

CPP_DEPS += \
./src/preprocessors/Aggregator.d \
./src/preprocessors/Preprocessor.d \
./src/preprocessors/Resolver.d 


# Each subdirectory must supply rules for building sources it contributes
src/preprocessors/%.o: ../src/preprocessors/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/jon/src/PL-stuff/TurboJS/lib/cute" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


