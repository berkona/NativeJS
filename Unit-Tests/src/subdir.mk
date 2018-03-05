################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CodeGenerator.cpp \
../src/Lexer.cpp \
../src/Parser.cpp \
../src/Resolver.cpp \
../src/Scope.cpp 

OBJS += \
./src/CodeGenerator.o \
./src/Lexer.o \
./src/Parser.o \
./src/Resolver.o \
./src/Scope.o 

CPP_DEPS += \
./src/CodeGenerator.d \
./src/Lexer.d \
./src/Parser.d \
./src/Resolver.d \
./src/Scope.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/jon/src/PL-stuff/TurboJS/lib/cute" -O0 -g3 -ftest-coverage -fprofile-arcs -Wall -c -fmessage-length=0 -frtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


