################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThreadX_M4/uart/UART.c 

OBJS += \
./ThreadX_M4/uart/UART.o 

C_DEPS += \
./ThreadX_M4/uart/UART.d 


# Each subdirectory must supply rules for building sources it contributes
ThreadX_M4/uart/%.o ThreadX_M4/uart/%.su ThreadX_M4/uart/%.cyclo: ../ThreadX_M4/uart/%.c ThreadX_M4/uart/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -DUSE_HAL_DRIVER -DSTM32L4R5xx -c -I../Core/Inc -I../Core/Inc/Application -I../ThreadX_M4/uart -I../AZURE_RTOS/App -I../USBX/App -I../USBX/Target -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/usbx/common/core/inc/ -I../Middlewares/ST/usbx/ports/generic/inc/ -I../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../Middlewares/ST/threadx/common/inc/ -I../Middlewares/ST/threadx/ports/cortex_m4/gnu/inc/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-ThreadX_M4-2f-uart

clean-ThreadX_M4-2f-uart:
	-$(RM) ./ThreadX_M4/uart/UART.cyclo ./ThreadX_M4/uart/UART.d ./ThreadX_M4/uart/UART.o ./ThreadX_M4/uart/UART.su

.PHONY: clean-ThreadX_M4-2f-uart

