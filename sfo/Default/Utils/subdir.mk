################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../Utils/arguments.cc \
../Utils/error.cc \
../Utils/utils.cc 

BCS += \
./Utils/arguments.bc \
./Utils/error.bc \
./Utils/utils.bc 

CC_DEPS += \
./Utils/arguments.d \
./Utils/error.d \
./Utils/utils.d 


# Each subdirectory must supply rules for building sources it contributes
Utils/%.bc: ../Utils/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: LLVM Clang++'
	clang++ -O2 -emit-llvm -g -Wall -c -fmessage-length=0 -std=c++0x -stdlib=libc++ -MMD -MP -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


