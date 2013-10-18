
SOURCES=main.c lcd_gpio_init.c stm32l_discovery_lcd_new.c
SOURCES+=../../libraries/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_adc.c ../../libraries/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_flash.c
SOURCES+=../../libraries/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_gpio.c ../../libraries/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_lcd.c
SOURCES+=../../libraries/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_pwr.c ../../libraries/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_rcc.c
SOURCES+=../../Utilities/stm32l_discovery_lcd.c #../../libraries/STM32_TouchSensing_Driver/src/stm32_tsl_api.c
#SOURCES+=../../libraries/STM32_TouchSensing_Driver/src/stm32_tsl_timebase.c ../../libraries/STM32_TouchSensing_Driver/src/stm32_tsl_services.c
#SOURCES+=../../libraries/STM32_TouchSensing_Driver/src/stm32_tsl_multichannelkey.c ../../libraries/STM32_TouchSensing_Driver/src/stm32l15x_tsl_ct_acquisition.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=gokarl

CFLAGS=-DSTM32L15XXB_128K -mthumb -mcpu=cortex-m3 -msoft-float
CFLAGS+=-I../inc
CFLAGS+=-I../../libraries/CMSIS/CM3/CoreSupport
CFLAGS+=-I../../libraries/CMSIS/CM3/DeviceSupport/ST/STM32L1xx
CFLAGS+=-I../../libraries/STM32L1xx_StdPeriph_Driver/inc
CFLAGS+=-I../../libraries/STM32_TouchSensing_Driver/inc
CFLAGS+=-I../../Utilities

CC=arm-none-eabi-gcc

all: $(SOURCES) $(EXECUTABLE)
	-echo "building all"

$(EXECUTABLE): $(OBJECTS)
	-echo "sources are $(SOURCES)"
	-echo "objects are $(OBJECTS)"
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(OBJECTS)


.PHONY: clean all
