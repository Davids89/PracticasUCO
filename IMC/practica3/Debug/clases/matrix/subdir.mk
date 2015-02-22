################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../clases/matrix/matrix.cpp 

OBJS += \
./clases/matrix/matrix.o 

CPP_DEPS += \
./clases/matrix/matrix.d 


# Each subdirectory must supply rules for building sources it contributes
clases/matrix/%.o: ../clases/matrix/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


