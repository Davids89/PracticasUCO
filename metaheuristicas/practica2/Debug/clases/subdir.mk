################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../clases/MMDP.cpp \
../clases/TSP.cpp 

OBJS += \
./clases/MMDP.o \
./clases/TSP.o 

CPP_DEPS += \
./clases/MMDP.d \
./clases/TSP.d 


# Each subdirectory must supply rules for building sources it contributes
clases/%.o: ../clases/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


