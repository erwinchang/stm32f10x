PREFIX := arm-none-eabi-

.PHONY: all clean

all: clock_ram.bin

clock.o: clock.c
	$(PREFIX)gcc -mcpu=cortex-m3 -mthumb -c clock.c -nostartfiles -o clock.o

vectors.o: vectors.S
	$(PREFIX)gcc -mcpu=cortex-m3 -mthumb -c -o vectors.o vectors.S

clock_ram.out: clock.o vectors.o stm32f103vet6.ld
	$(PREFIX)ld -T stm32f103vet6.ld -o $@ clock.o vectors.o

clock_ram.bin: clock_ram.out
	$(PREFIX)objcopy -O binary $< $@

clean:
	rm -f *.o *.out *.bin
