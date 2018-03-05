################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CodeGenerator.cpp \
../src/ExprGenerator.cpp \
../src/JSObject.cpp \
../src/Lexer.cpp \
../src/Parser.cpp \
../src/TurboJS.cpp \
../src/util.cpp 

BCS += \
./src/CodeGenerator.bc \
./src/ExprGenerator.bc \
./src/JSObject.bc \
./src/Lexer.bc \
./src/Parser.bc \
./src/TurboJS.bc \
./src/util.bc 

CPP_DEPS += \
./src/CodeGenerator.d \
./src/ExprGenerator.d \
./src/JSObject.d \
./src/Lexer.d \
./src/Parser.d \
./src/TurboJS.d \
./src/util.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.bc: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: LLVM Clang++'
	clang++ -I"/home/jon/src/PL-stuff/TurboJS/lib/cute" -I/home/jon/src/llvm-3.5.2/include/llvm -O0 -emit-llvm -g3 -Wall -c -fmessage-length=0 -MMD -MP -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


