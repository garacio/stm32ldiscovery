arm-none-eabi-gcc -Wall -g -std=c99 -O0 -mlittle-endian -mcpu=cortex-m3 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=soft -fsingle-precision-constant -ffunction-sections -fdata-sections -Wl,--gc-sections -Wl,-Map=demo.map -I inc -I ../../lib -I ../../lib/board -I ../../lib/board -I ../../lib/STM32L1xx_StdPeriph_Driver/inc -I ../../lib/CMSIS/CM3/CoreSupport -I ../../lib/CMSIS/CM3/DeviceSupport/ST/STM32L1xx src/main.c ../../lib/board/stm32l_discovery_lcd.c ../../lib/board/discover_functions.c ../../lib/board/icc_measure.c ../../lib/board/icc_measure_Ram.c ../../lib/board/stm32l1xx_it.c ../../lib/board/system_stm32l1xx.c ../../lib/CMSIS/CM3/DeviceSupport/ST/STM32L1xx/startup/gcc_ride7/startup_stm32l1xx_md.s -o demo.elf -L../../lib/STM32L1xx_StdPeriph_Driver -lstm32l1 -L../../lib/board/ldscripts -Tstm32l152rb.ld
