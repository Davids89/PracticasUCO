################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../clases/maxent-3.0/lbfgs.cpp \
../clases/maxent-3.0/maxent.cpp \
../clases/maxent-3.0/owlqn.cpp \
../clases/maxent-3.0/sgd.cpp 

C_SRCS += \
../clases/maxent-3.0/modeldata.c 

OBJS += \
./clases/maxent-3.0/lbfgs.o \
./clases/maxent-3.0/maxent.o \
./clases/maxent-3.0/modeldata.o \
./clases/maxent-3.0/owlqn.o \
./clases/maxent-3.0/sgd.o 

C_DEPS += \
./clases/maxent-3.0/modeldata.d 

CPP_DEPS += \
./clases/maxent-3.0/lbfgs.d \
./clases/maxent-3.0/maxent.d \
./clases/maxent-3.0/owlqn.d \
./clases/maxent-3.0/sgd.d 


# Each subdirectory must supply rules for building sources it contributes
clases/maxent-3.0/%.o: ../clases/maxent-3.0/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

clases/maxent-3.0/%.o: ../clases/maxent-3.0/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


