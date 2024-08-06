################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Application/DebugPort.c \
../Core/Src/Application/Init_App.c \
../Core/Src/Application/TerminalEmulatorSupport.c \
../Core/Src/Application/TestApp_USB_CDC.c 

OBJS += \
./Core/Src/Application/DebugPort.o \
./Core/Src/Application/Init_App.o \
./Core/Src/Application/TerminalEmulatorSupport.o \
./Core/Src/Application/TestApp_USB_CDC.o 

C_DEPS += \
./Core/Src/Application/DebugPort.d \
./Core/Src/Application/Init_App.d \
./Core/Src/Application/TerminalEmulatorSupport.d \
./Core/Src/Application/TestApp_USB_CDC.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Application/%.o Core/Src/Application/%.su Core/Src/Application/%.cyclo: ../Core/Src/Application/%.c Core/Src/Application/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -DUSE_HAL_DRIVER -DSTM32L4R5xx -c -I../Core/Inc -I../Core/Inc/Application -I../ThreadX_M4/uart -I../AZURE_RTOS/App -I../USBX/App -I../USBX/Target -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/usbx/common/core/inc/ -I../Middlewares/ST/usbx/ports/generic/inc/ -I../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../Middlewares/ST/threadx/common/inc/ -I../Middlewares/ST/threadx/ports/cortex_m4/gnu/inc/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Application

clean-Core-2f-Src-2f-Application:
	-$(RM) ./Core/Src/Application/DebugPort.cyclo ./Core/Src/Application/DebugPort.d ./Core/Src/Application/DebugPort.o ./Core/Src/Application/DebugPort.su ./Core/Src/Application/Init_App.cyclo ./Core/Src/Application/Init_App.d ./Core/Src/Application/Init_App.o ./Core/Src/Application/Init_App.su ./Core/Src/Application/TerminalEmulatorSupport.cyclo ./Core/Src/Application/TerminalEmulatorSupport.d ./Core/Src/Application/TerminalEmulatorSupport.o ./Core/Src/Application/TerminalEmulatorSupport.su ./Core/Src/Application/TestApp_USB_CDC.cyclo ./Core/Src/Application/TestApp_USB_CDC.d ./Core/Src/Application/TestApp_USB_CDC.o ./Core/Src/Application/TestApp_USB_CDC.su

.PHONY: clean-Core-2f-Src-2f-Application

