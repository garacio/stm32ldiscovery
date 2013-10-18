CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar

###########################################

vpath %.c STM32_TouchSensing_Driver/src

CFLAGS  = -DSTM32L15XXB_128K -g -O0 -Wall
CFLAGS += -mlittle-endian -mcpu=cortex-m3 -mthumb
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=soft
CFLAGS += -ffreestanding -nostdlib
CFLAGS += -includestm32l1xx_conf.h -I../CMSIS/CM3/CoreSupport/ -I../CMSIS/CM3/DeviceSupport/ST/STM32L1xx/ -Iinc -I.. -I../STM32L1xx_StdPeriph_Driver/inc/


SRCS = src/stm32_tsl_api.c     				src/stm32_tsl_multichannelkey.c    src/stm32_tsl_services.c \
	   src/stm32_tsl_singlechannelkey.c 		src/stm32_tsl_timebase.c 	  src/stm32l15x_tsl_ct_acquisition.c



OBJS = $(SRCS:.c=.o)

.PHONY: libstm32tsl.a

all: libstm32tsl.a

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

libstm32tsl.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) libstm32tsl.a
