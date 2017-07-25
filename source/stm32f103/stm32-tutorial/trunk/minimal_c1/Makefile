PREFIX := arm-none-eabi-
.PHONY: all clean

all: blink.bin

blink.o: blink.c
	$(PREFIX)gcc -mcpu=cortex-m3 -mthumb -nostartfiles -c blink.c -o blink.o

blink.out: blink.o simple.ld
	$(PREFIX)ld -T simple.ld -o blink.out blink.o

blink.bin: blink.out
	$(PREFIX)objcopy -j .text -O binary blink.out blink.bin

clean:
	rm -f *.o *.out *.bin
