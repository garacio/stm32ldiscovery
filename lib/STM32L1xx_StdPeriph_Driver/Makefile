CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar

###########################################

vpath %.c STM32L1xx_StdPeriph_Driver/src

CFLAGS  = -g -O0 -Wall
CFLAGS += -mlittle-endian -mcpu=cortex-m3 -mthumb
CFLAGS += -mfpu=fpv4-sp-d16 -fsingle-precision-constant
CFLAGS += -ffreestanding -nostdlib
CFLAGS += -includestm32l1xx_conf.h -I../CMSIS/CM3/CoreSupport/ -I../CMSIS/CM3/DeviceSupport/ST/STM32L1xx/ -Iinc -I..


SRCS = src/stm32l1xx_adc.c     src/stm32l1xx_dma.c    src/stm32l1xx_syscfg.c \
	   src/stm32l1xx_usart.c   src/stm32l1xx_exti.c   src/stm32l1xx_tim.c 				src/stm32l1xx_syscfg.c \
       src/stm32l1xx_comp.c    src/stm32l1xx_flash.c  src/stm32l1xx_flash_ramfunc.c 	src/stm32l1xx_pwr.c \
       src/stm32l1xx_crc.c     src/stm32l1xx_gpio.c   src/stm32l1xx_rcc.c 				src/stm32l1xx_lcd.c \
       src/stm32l1xx_dac.c     src/stm32l1xx_i2c.c    src/stm32l1xx_rtc.c \
       src/stm32l1xx_dbgmcu.c  src/stm32l1xx_iwdg.c   src/stm32l1xx_spi.c 				src/stm32l1xx_wwdg.c


OBJS = $(SRCS:.c=.o)

.PHONY: libstm32l1.a

all: libstm32l1.a

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

libstm32l1.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) libstm32l1.a
