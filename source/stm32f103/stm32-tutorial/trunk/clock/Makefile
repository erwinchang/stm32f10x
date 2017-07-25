PREFIX := arm-none-eabi-

.PHONY: all clean

all: clock.bin

clock.o: clock.c
	$(PREFIX)gcc -mcpu=cortex-m3 -mthumb -c clock.c -nostartfiles -o clock.o

isr.o: isr.c
	$(PREFIX)gcc -mcpu=cortex-m3 -mthumb -c isr.c -nostartfiles -o isr.o

clock.out: clock.o isr.o stm32f103vet6.ld
	$(PREFIX)ld -T stm32f103vet6.ld -o clock.out clock.o isr.o

clock.bin: clock.out
	$(PREFIX)objcopy -O binary clock.out clock.bin

clean:
	rm -f *.o *.out *.bin
