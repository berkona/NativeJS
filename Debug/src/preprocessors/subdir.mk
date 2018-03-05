################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/preprocessors/Aggregator.cpp \
../src/preprocessors/Preprocessor.cpp \
../src/preprocessors/Resolver.cpp 

BCS += \
./src/preprocessors/Aggregator.bc \
./src/preprocessors/Preprocessor.bc \
./src/preprocessors/Resolver.bc 

CPP_DEPS += \
./src/preprocessors/Aggregator.d \
./src/preprocessors/Preprocessor.d \
./src/preprocessors/Resolver.d 


# Each subdirectory must supply rules for building sources it contributes
src/preprocessors/%.bc: ../src/preprocessors/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: LLVM Clang++'
	clang++ -I"/home/jon/src/PL-stuff/TurboJS/lib/cute" -I/home/jon/src/llvm-3.5.2/include/llvm -O0 -emit-llvm -g3 -Wall -c -fmessage-length=0 -MMD -MP -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


