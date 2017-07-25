PREFIX := arm-none-eabi-

.PHONY: all clean

all: blink.bin

blink.o: blink.c
	$(PREFIX)gcc -mcpu=cortex-m3 -mthumb -c blink.c -nostartfiles -o blink.o

blink.out: blink.o stm32f103vet6.ld
	$(PREFIX)ld -T stm32f103vet6.ld -o blink.out blink.o

blink.bin: blink.out
	$(PREFIX)objcopy -O binary blink.out blink.bin

clean:
	rm -f *.o *.out *.bin
