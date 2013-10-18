# put your *.o targets here, make should handle the rest!
SRCS = main.c lcd_gpio_init.c stm32l_discovery_lcd_new.c system_stm32l1xx.c

# lib root
LIB=../../lib

# specific libs
# BRD=$(LIB)/board
# IMU=$(LIB)/board

# additional includes
INCL=-I $(BRD) -I $(IMU)

# board library
# SRCS += \
 # $(BRD)/stm32l_discovery_lcd.c


SSRCS += \
 $(LIB)/CMSIS/CM3/DeviceSupport/ST/STM32L1xx/startup/gcc_ride7/startup_stm32l1xx_md.s

 # imu library
# SRCS += \
#  $(IMU)/discover_functions.c \
#  $(IMU)/icc_measure.c \
#  $(IMU)/icc_measure_Ram.c \
#  $(IMU)/stm32l1xx_it.c \
 # $(IMU)/system_stm32l1xx.c

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)
PROJ_NAME=lcd

# Location of the linker scripts
LDSCRIPT_INC=$(LIB)/board/ldscripts

# location of OpenOCD Board .cfg files (only used with 'make program')
OPENOCD_BOARD_DIR=board

CPU=stm32l1

# misc config files
EXTRA=../../extra

# Configuration (cfg) file containing programming directives for OpenOCD
OPENOCD_PROC_FILE=$(EXTRA)/$(CPU)-openocd.cfg

# GDB startup file for "debug" target
GDB_CMDS_FILE=$(EXTRA)/start.gdb

# that's it, no need to change anything below this line!

###################################################

# upper case CPU used to access CMIS
CPUx=`echo $(CPU) | tr '[:lower:]' '[:upper:]'`

CC=arm-none-eabi-gcc
GDB=arm-none-eabi-gdb
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
SIZE=arm-none-eabi-size

CFLAGS  = -Wall -g -O0 -DSTM32L15XXB_128K
CFLAGS += -mlittle-endian -mcpu=cortex-m3 -mthumb

# floating point options
CFLAGS += -mfpu=fpv4-sp-d16 -fsingle-precision-constant

CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wl,--gc-sections -Wl,-Map=$(PROJ_NAME).map

###################################################

vpath %.c src
vpath %.a $(STD_PERIPH_LIB)
# vpath %.a $(STD_TSL)

ROOT=$(shell pwd)

CFLAGS += -I inc -I $(LIB) $(INCL)

STD_PERIPH_LIB=$(LIB)/STM32L1xx_StdPeriph_Driver
# STD_TSL=$(LIB)/STM32_TouchSensing_Driver

CFLAGS += -I $(LIB)/CMSIS/Include
CFLAGS += -I $(LIB)/CMSIS/CM3/DeviceSupport/ST/STM32L1xx
CFLAGS += -I $(STD_PERIPH_LIB)/inc
CFLAGS += -I $(LIB)/STM32_TouchSensing_Driver/inc

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj

all: lib proj

lib:
	@echo BUILDING $(STD_PERIPH_LIB)
	$(MAKE) -C $(STD_PERIPH_LIB)
	# @echo BUILDING $(STD_TSL)
	# $(MAKE) -C $(STD_TSL)

proj: 	$(PROJ_NAME).elf $(PROJ_NAME).bin $(PROJ_NAME).hex $(PROJ_NAME).lst

$(PROJ_NAME).elf: $(SRCS) $(SSRCS)
	$(CC) $(CFLAGS) $^ -o $@ -L$(STD_PERIPH_LIB) -l$(CPU) -L$(LDSCRIPT_INC) -T$(CPU)52rb.ld
	#$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	#$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	#$(OBJDUMP) -St $(PROJ_NAME).elf >$(PROJ_NAME).lst
	$(SIZE) $(PROJ_NAME).elf

$(PROJ_NAME).bin: $(PROJ_NAME).elf
	$(OBJCOPY) -O binary $< $@

$(PROJ_NAME).hex: $(PROJ_NAME).elf
	$(OBJCOPY) -O ihex $< $@

$(PROJ_NAME).lst: $(PROJ_NAME).elf
	$(OBJDUMP) -St $< >$@

program: $(PROJ_NAME).bin
	killall openocd || True
	openocd -f $(OPENOCD_BOARD_DIR)/$(CPU)discovery.cfg -f $(OPENOCD_PROC_FILE) -c "stm_flash `pwd`/$(PROJ_NAME).bin" -c shutdown

run: $(PROJ_NAME).bin
	killall openocd || True
	openocd -f $(OPENOCD_BOARD_DIR)/$(CPU)discovery.cfg -f $(OPENOCD_PROC_FILE) -c "stm_flash `pwd`/$(PROJ_NAME).bin;reset run" -c shutdown

startocd:
	killall openocd || True
	openocd -f $(OPENOCD_BOARD_DIR)/$(CPU)discovery.cfg -f $(OPENOCD_PROC_FILE) &


debug: $(PROJ_NAME).elf
	$(MAKE) startocd
	$(GDB) -x $(GDB_CMDS_FILE) $<

debugt: $(PROJ_NAME).elf
	$(MAKE) startocd
	$(GDB)tui -x $(GDB_CMDS_FILE) $<

ddd: $(PROJ_NAME).elf
	$(MAKE) startocd
	ddd --eval-command="target remote localhost:3333" --debugger $(GDB) -x $(GDB_CMDS_FILE) $<

install:
	arm-none-eabi-gdb --eval-command="target extended-remote :4242" --eval-command="load $(PROJ_NAME).elf" --eval-command="quit" --batch

clean:
	rm -f $(OBJS)
	find ./ -name '*~' | xargs rm -f
	rm -f *.o
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
	rm -f $(PROJ_NAME).map
	rm -f $(PROJ_NAME).lst

reallyclean: clean
	$(MAKE) -C $(STD_PERIPH_LIB) clean
