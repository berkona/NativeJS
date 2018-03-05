################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../test/LexerTest.cpp \
../test/ParserTest.cpp \
../test/runner.cpp 

OBJS += \
./test/LexerTest.o \
./test/ParserTest.o \
./test/runner.o 

CPP_DEPS += \
./test/LexerTest.d \
./test/ParserTest.d \
./test/runner.d 


# Each subdirectory must supply rules for building sources it contributes
test/%.o: ../test/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/jon/src/PL-stuff/TurboJS/lib/cute" -O0 -g3 -ftest-coverage -fprofile-arcs -Wall -c -fmessage-length=0 -frtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


