################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../test/LexerTest.cpp \
../test/ParserTest.cpp 

BCS += \
./test/LexerTest.bc \
./test/ParserTest.bc 

CPP_DEPS += \
./test/LexerTest.d \
./test/ParserTest.d 


# Each subdirectory must supply rules for building sources it contributes
test/%.bc: ../test/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: LLVM Clang++'
	clang++ -I"/home/jon/src/PL-stuff/TurboJS/lib/cute" -I/home/jon/src/llvm-3.5.2/include/llvm -O0 -emit-llvm -g3 -Wall -c -fmessage-length=0 -MMD -MP -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


