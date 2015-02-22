################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../clases/CWP.cpp \
../clases/MMDP.cpp \
../clases/TSP.cpp 

OBJS += \
./clases/CWP.o \
./clases/MMDP.o \
./clases/TSP.o 

CPP_DEPS += \
./clases/CWP.d \
./clases/MMDP.d \
./clases/TSP.d 


# Each subdirectory must supply rules for building sources it contributes
clases/%.o: ../clases/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


