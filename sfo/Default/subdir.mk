################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../main.cc \
../sfotutorialSmall.cc \
../timingAnalysis.cc 

BCS += \
./main.bc \
./sfotutorialSmall.bc \
./timingAnalysis.bc 

CC_DEPS += \
./main.d \
./sfotutorialSmall.d \
./timingAnalysis.d 


# Each subdirectory must supply rules for building sources it contributes
%.bc: ../%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: LLVM Clang++'
	clang++ -O2 -emit-llvm -g -Wall -c -fmessage-length=0 -std=c++0x -stdlib=libc++ -MMD -MP -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


