################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../asm/codelist.cpp \
../asm/input_tokenizer.cpp \
../asm/instr_list.cpp \
../asm/instruction.cpp \
../asm/main.cpp \
../asm/toyasm.cpp 

OBJS += \
./asm/codelist.o \
./asm/input_tokenizer.o \
./asm/instr_list.o \
./asm/instruction.o \
./asm/main.o \
./asm/toyasm.o 

CPP_DEPS += \
./asm/codelist.d \
./asm/input_tokenizer.d \
./asm/instr_list.d \
./asm/instruction.d \
./asm/main.d \
./asm/toyasm.d 


# Each subdirectory must supply rules for building sources it contributes
asm/%.o: ../asm/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -I"/home/bmelikant/project/toypc-asm/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


