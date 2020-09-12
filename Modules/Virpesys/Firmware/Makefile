RM := rm -rf

INC_DIRS += -IDrivers/STM32F3xx_HAL_Driver/Inc
INC_DIRS += -IDrivers/CMSIS/ST/STM32F3xx/Include
INC_DIRS += -IDrivers/CMSIS/Includei

C_SRCS += \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_adc_ex.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dac.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dac_ex.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_adc.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c_ex.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart.c \
Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart_ex.c 

S_SRCS += \
Core/Startup/startup_stm32f303cctx.s 

INC_DIRS += -ICore/Inc

C_SRCS += \
Core/Src/stm32f3xx_hal_msp.c \
Core/Src/stm32f3xx_it.c \
Core/Src/syscalls.c \
Core/Src/sysmem.c \
Core/Src/system_stm32f3xx.c 

C_SRCS += \
main.c 

EXECUTABLES += \
virpesys-fw.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
virpesys-fw.list \

OBJCOPY_BIN += \
virpesys-fw.bin \

GCC_FLAGS += -mcpu=cortex-m4 
GCC_FLAGS += -T"C:\git\fibration\modules\nco\nco-fw\STM32F303CCTX_FLASH.ld" 
GCC_FLAGS += --specs=nosys.specs 
GCC_FLAGS += -Wl,-Map="virpesys-fw.map" 
GCC_FLAGS += -Wl,--gc-sections 
GCC_FLAGS += --static 
GCC_FLAGS += --specs=nano.specs 
GCC_FLAGS += -mfpu=fpv4-sp-d16 
GCC_FLAGS += -mfloat-abi=hard 
GCC_FLAGS += -mthumb 
GCC_FLAGS += -Wl,--start-group 
GCC_FLAGS += -lc -lm 
GCC_FLAGS += -Wl,--end-group 

# All Target
all: virpesys-fw.elf secondary-outputs

# Tool invocations
virpesys-fw.elf: $(C_SRCS) $(S_SRCS) $(USER_OBJS) $(INC_DIRS) C:\git\fibration\modules\nco\nco-fw\STM32F303CCTX_FLASH.ld
	arm-none-eabi-gcc -o "virpesys-fw.elf" $(C_SRCS) $(S_SRCS) $(INC_DIRS) $(GCC_FLAGS)
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

virpesys-fw.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "virpesys-fw.list"
	@echo 'Finished building: $@'
	@echo ' '

virpesys-fw.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "virpesys-fw.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:
