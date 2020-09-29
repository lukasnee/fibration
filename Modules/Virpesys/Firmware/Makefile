# Fibration Project Makefile
# Copyright (C) 2020 Lukas Neverauskis

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

#===============================================================================
# Common
#===============================================================================

PROJECT=Virpesys

TARGET=$(PROJECT)
FIB_ROOT=../../..
PROJECT_DIR=.# makefile should be at project software root directory
LIBRARIES=$(FIB_ROOT)/Libraries

FIBLIB=$(LIBRARIES)/Fibration
FIBLIB_COMPONENTS=$(FIBLIB)/Components
FIBLIB_PARTS=$(FIBLIB_COMPONENTS)/Parts
FIBLIB_HARDWARE=$(FIBLIB_PARTS)/Hardware
FIBLIB_DRIVERS=$(FIBLIB_HARDWARE)/Drivers

#===============================================================================
# Build Tools
#===============================================================================

RM := rm -rf
CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
CP=arm-none-eabi-objcopy
OD=arm-none-eabi-objdump
SE=arm-none-eabi-size
SF=st-flash

#===============================================================================
# Main Source of the project
#===============================================================================

INCLUDES += -IInclude/

STM32_FAMILY=stm32f3xx
STM32_FAMILY_CAP=STM32F3xx

PROJECT_SRC=Source

S_SRCS = $(PROJECT_DIR)/startup_stm32f303cctx.s 

C_SRCS = $(PROJECT_SRC)/main.c
C_SRCS += $(PROJECT_SRC)/$(STM32_FAMILY)_hal_msp.c
C_SRCS += $(PROJECT_SRC)/$(STM32_FAMILY)_it.c
C_SRCS += $(PROJECT_SRC)/syscalls.c
C_SRCS += $(PROJECT_SRC)/sysmem.c
C_SRCS += $(PROJECT_SRC)/system_$(STM32_FAMILY).c

#===============================================================================
# STM32 Hardware Drivers (CMSIS/HAL/LL)
#===============================================================================

# CMSIS stuff

INCLUDES += -I$(FIBLIB_DRIVERS)/CMSIS/Device/ST/$(STM32_FAMILY_CAP)/Include
INCLUDES += -I$(FIBLIB_DRIVERS)/CMSIS/Include

# Hardware Abstraction Layer (HAL) stuff

STM32_HAL_DIR=$(FIBLIB_DRIVERS)/$(STM32_FAMILY_CAP)_HAL_Driver
STM32_HAL=$(STM32_HAL_DIR)/Src/$(STM32_FAMILY)_hal
INCLUDES += -I$(STM32_HAL_DIR)/Inc
INCLUDES += -I$(STM32_HAL_DIR)/Legacy

C_SRCS += $(STM32_HAL).c
C_SRCS += $(STM32_HAL)_adc.c
C_SRCS += $(STM32_HAL)_adc_ex.c
#C_SRCS += $(STM32_HAL)_can.c
#C_SRCS += $(STM32_HAL)_cec.c
#C_SRCS += $(STM32_HAL)_comp.c
C_SRCS += $(STM32_HAL)_cortex.c
#C_SRCS += $(STM32_HAL)_crc.c
#C_SRCS += $(STM32_HAL)_crc_ex.c
C_SRCS += $(STM32_HAL)_dac.c
C_SRCS += $(STM32_HAL)_dac_ex.c
C_SRCS += $(STM32_HAL)_dma.c
#C_SRCS += $(STM32_HAL)_exti.c
C_SRCS += $(STM32_HAL)_flash.c
C_SRCS += $(STM32_HAL)_flash_ex.c
C_SRCS += $(STM32_HAL)_gpio.c
#C_SRCS += $(STM32_HAL)_hrtim.c
#C_SRCS += $(STM32_HAL)_i2c.c
#C_SRCS += $(STM32_HAL)_i2c_ex.c
C_SRCS += $(STM32_HAL)_i2s.c
C_SRCS += $(STM32_HAL)_i2s_ex.c
#C_SRCS += $(STM32_HAL)_irda.c
#C_SRCS += $(STM32_HAL)_iwdg.c
#C_SRCS += $(STM32_HAL)_msp_template.c
#C_SRCS += $(STM32_HAL)_nand.c
#C_SRCS += $(STM32_HAL)_nor.c
#C_SRCS += $(STM32_HAL)_opamp.c
#C_SRCS += $(STM32_HAL)_opamp_ex.c
#C_SRCS += $(STM32_HAL)_pccard.c
#C_SRCS += $(STM32_HAL)_pcd.c
#C_SRCS += $(STM32_HAL)_pcd_ex.c
#C_SRCS += $(STM32_HAL)_pwr.c
#C_SRCS += $(STM32_HAL)_pwr_ex.c
C_SRCS += $(STM32_HAL)_rcc.c
C_SRCS += $(STM32_HAL)_rcc_ex.c
#C_SRCS += $(STM32_HAL)_rtc.c
#C_SRCS += $(STM32_HAL)_rtc_ex.c
C_SRCS += $(STM32_HAL)_sdadc.c
#C_SRCS += $(STM32_HAL)_smartcard.c
#C_SRCS += $(STM32_HAL)_smartcard_ex.c
#C_SRCS += $(STM32_HAL)_smbus.c
#C_SRCS += $(STM32_HAL)_spi.c
#C_SRCS += $(STM32_HAL)_spi_ex.c
#C_SRCS += $(STM32_HAL)_sram.c
C_SRCS += $(STM32_HAL)_tim.c
#C_SRCS += $(STM32_HAL)_timebase_rtc_alarm_template.c
#C_SRCS += $(STM32_HAL)_timebase_rtc_wakeup_template.c
#C_SRCS += $(STM32_HAL)_timebase_tim_template.c
C_SRCS += $(STM32_HAL)_tim_ex.c
#C_SRCS += $(STM32_HAL)_tsc.c
C_SRCS += $(STM32_HAL)_uart.c
C_SRCS += $(STM32_HAL)_uart_ex.c
#C_SRCS += $(STM32_HAL)_usart.c
#C_SRCS += $(STM32_HAL)_usart_ex.c
#C_SRCS += $(STM32_HAL)_wwdg.c

# Low Level driver stuff

STM32_LL=$(STM32_HAL_DIR)/$(STM32_FAMILY)_ll

#C_SRCS += $(STM32_LL)_adc.c
#C_SRCS += $(STM32_LL)_comp.c
#C_SRCS += $(STM32_LL)_crc.c
#C_SRCS += $(STM32_LL)_dac.c
#C_SRCS += $(STM32_LL)_dma.c
#C_SRCS += $(STM32_LL)_exti.c
#C_SRCS += $(STM32_LL)_fmc.c
#C_SRCS += $(STM32_LL)_gpio.c
#C_SRCS += $(STM32_LL)_hrtim.c
#C_SRCS += $(STM32_LL)_i2c.c
#C_SRCS += $(STM32_LL)_opamp.c
#C_SRCS += $(STM32_LL)_pwr.c
#C_SRCS += $(STM32_LL)_rcc.c
#C_SRCS += $(STM32_LL)_rtc.c
#C_SRCS += $(STM32_LL)_spi.c
#C_SRCS += $(STM32_LL)_tim.c
#C_SRCS += $(STM32_LL)_usart.c
#C_SRCS += $(STM32_LL)_usb.c
#C_SRCS += $(STM32_LL)_utils.c

#===============================================================================
# Build Flags
#===============================================================================

CFLAGS = -mcpu=cortex-m4 
CFLAGS += -mlittle-endian
CFLAGS += -mthumb -mthumb-interwork
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -std=gnu99 
CFLAGS += -g 
CFLAGS += -O2 
CFLAGS += -Wall
CFLAGS += -DSTM32F303xC
CFLAGS += -T"STM32F303CCTX_FLASH.ld" 

CFLAGS += -mfpu=fpv4-sp-d16
CFLAGS += --specs=nosys.specs
CFLAGS += --specs=nano.specs
CFLAGS += -Wl,-Map="$(TARGET).map" 
CFLAGS += -Wl,--gc-sections 
CFLAGS += --static 
CFLAGS += -mfloat-abi=hard 
CFLAGS += -Wl,--start-group 
CFLAGS += -lc -lm 
CFLAGS += -Wl,--end-group 

#===============================================================================
# Build Targets
#===============================================================================

.PHONY: $(TARGET)

$(TARGET): $(TARGET).elf

$(TARGET).elf: $(C_SRCS) $(S_SRCS)
		$(CC) $(INCLUDES) $(CFLAGS) $^ -o $@
		$(CP) -O binary $(TARGET).elf $(TARGET).bin

clean:
		rm -f *.o $(TARGET).elf $(TARGET).bin

flash:
		$(SF) write $(TARGET).bin 0x8000000
