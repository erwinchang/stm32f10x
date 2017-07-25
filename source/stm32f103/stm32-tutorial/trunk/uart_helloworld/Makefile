PREFIX := arm-none-eabi-

.PHONY: all clean

all: helloworld.bin

uart_helloworld.o: uart_helloworld.c
	$(PREFIX)gcc -mcpu=cortex-m3 -mthumb -c uart_helloworld.c -nostartfiles -o uart_helloworld.o

isr.o: isr.c
	$(PREFIX)gcc -mcpu=cortex-m3 -mthumb -c isr.c -nostartfiles -o isr.o

helloworld.out: uart_helloworld.o isr.o stm32f103vet6.ld
	$(PREFIX)ld -T stm32f103vet6.ld -o helloworld.out uart_helloworld.o isr.o

helloworld.bin: helloworld.out
	$(PREFIX)objcopy -O binary helloworld.out helloworld.bin

clean:
	rm -f *.o *.out *.bin
