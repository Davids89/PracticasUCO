################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../clases/kmeans/kmeans.cpp 

OBJS += \
./clases/kmeans/kmeans.o 

CPP_DEPS += \
./clases/kmeans/kmeans.d 


# Each subdirectory must supply rules for building sources it contributes
clases/kmeans/%.o: ../clases/kmeans/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


