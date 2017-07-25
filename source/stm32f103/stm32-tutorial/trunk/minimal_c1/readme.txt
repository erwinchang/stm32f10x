This is a document I found with google, enjoy!

======Writing a minimal C program for the STM32 Primer======

[[arm_cortex-m3:stm32:basics|Cortex-M3 basics]] and [[arm_cortex-m3:stm32:stm32-circle|Primer programming with OpenOCD]] provides the background material for this section.

=====Blink the RED LED on the Primer!=====
The Primer-1's RED LED is connected to GPIO Port B, pin 9 (PB9). Putting this on requires:

  *Enabling the peripheral clock to GPIO PORTB
  *Configuring PB9 as a push-pull output 
  *Putting ON PB9 by writing to the BSRR (Bit Set/Reset register)

All these things will be done by our //main// program whose address will be stored at memory location 0x4. We will store 0x20001000 at 0x0 - the processor will use this value to initialize the stack pointer register.

====Installing the toolchain====
CodeSourcery provides an easy to use installer for their GNU ARM toolchain - [[http://www.codesourcery.com/sgpp/lite/arm/portal/release642|download Sourcery G++ Lite 2008q3-66 from here]] it and simply execute  the .bin file to get started with the installation.

====Building the code====
<code c>
/*
 * Red LED on PB9, Green on PB8. We will put on Red
 *
 * GPIOB_CRH is initially 0x44444444 (reset state, all pins inputs
 * and floating)
 *
 * We will change GPIOB_CRH to 0x44444414 (PB9 push-pull output)
 *
 * It is possible to make PB9 go high without touching any of the
 * other pins by using the GPIOB_BSRR - we will set the 9th bit of
 * this register to set PB9; ie, we will write (1 << 9) to GPIOB_BSRR
 *
 * GPIOB registers start at 0x40010c00.
 *
 * GPIOB_CRH is at offset 0x4 and GPIOB_BSRR is at offset 0x10
 *
 * The peripheral clock register has to be configured to enable clock
 * to GPIOB. This is done by setting bit 3 of RCC_APB2ENR.
 *
 * The clock registers start at 0x40021000 and RCC_APB2ENR is at 
 * offset 0x018.
 *
 * The STM32F103RB processor has 128Kb flash and 20Kb RAM.
 *
 * 20Kb is 0x5000. RAM starts at 0x20000000. Lets initialize stack
 * pointer to 0x20001000. (4Kb stack)
 *
 */

#define GPIOB_CRH  (*((volatile unsigned long*)(0x40010c00 + 0x4)))
#define GPIOB_BSRR  (*((volatile unsigned long*)(0x40010c00 + 0x10)))
#define RCC_APB2ENR  (*((volatile unsigned long*)(0x40021000 + 0x018)))

        
__asm__(".word 0x20001000");
__asm__(".word main");
main()
{
        unsigned int c = 0;

        RCC_APB2ENR = (1 << 3);
        GPIOB_CRH = 0x44444414;
        
        while(1) {
                GPIOB_BSRR = (1 << 9); // ON
                for(c = 0; c < 100000; c++);
                GPIOB_BSRR = (1 << 25); // OFF
                for(c = 0; c < 100000; c++);
        }
                
} 
</code> 

We need a //linker script// to compile the code into an a.out (it's also possible to manage without a linker script with a slightly different approach). Here is a sample script, //simple.lds//

  SECTIONS {
        . = 0x0;
        .text : {
                *(.text)
        }
  }

The code is compiled into an a.out by running:

  arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb  -c blink.c
  arm-none-eabi-ld -T simple.lds blink.o

It's essential that we peek into the executable to verify that things are laid out in memory  the way they should be - this is done by running //objdump//:

  arm-none-eabi-objdump -D a.out 

This is part of the output we get:

<code asm>
a.out:     file format elf32-littlearm

Disassembly of section .text:

00000000 <main-0x8>:
   0:   1000            asrs    r0, r0, #32
   2:   2000            movs    r0, #0
   4:   0009            lsls    r1, r1, #0
        ...

00000008 <main>:
   8:   b480            push    {r7}
   a:   b083            sub     sp, #12
   c:   af00            add     r7, sp, #0
   e:   f04f 0300       mov.w   r3, #0  ; 0x0
  12:   607b            str     r3, [r7, #4]
  14:   f241 0318       movw    r3, #4120       ; 0x1018
  18:   f2c4 0302       movt    r3, #16386      ; 0x4002
  1c:   f04f 0208       mov.w   r2, #8  ; 0x8
  20:   601a            str     r2, [r3, #0]
  22:   f640 4204       movw    r2, #3076       ; 0xc04
  26:   f2c4 0201       movt    r2, #16385      ; 0x4001
  2a:   f244 4314       movw    r3, #17428      ; 0x4414
  2e:   f2c4 4344       movt    r3, #17476      ; 0x4444
  32:   6013            str     r3, [r2, #0]
  34:   f640 4310       movw    r3, #3088       ; 0xc10
  38:   f2c4 0301       movt    r3, #16385      ; 0x4001
  3c:   f44f 7200       mov.w   r2, #512        ; 0x200
  40:   601a            str     r2, [r3, #0]
  42:   f04f 0300       mov.w   r3, #0  ; 0x0
  46:   607b            str     r3, [r7, #4]
  48:   e003            b.n     52 <main+0x4a>
  4a:   687b            ldr     r3, [r7, #4]
  4c:   f103 0301       add.w   r3, r3, #1      ; 0x1
  50:   607b            str     r3, [r7, #4]
  52:   687a            ldr     r2, [r7, #4]
  54:   f248 639f       movw    r3, #34463      ; 0x869f
  58:   f2c0 0301       movt    r3, #1  ; 0x1
  5c:   429a            cmp     r2, r3
  5e:   d9f4            bls.n   4a <main+0x42>
  60:   f640 4310       movw    r3, #3088       ; 0xc10
  64:   f2c4 0301       movt    r3, #16385      ; 0x4001
  68:   f04f 7200       mov.w   r2, #33554432   ; 0x2000000
  6c:   601a            str     r2, [r3, #0]
  6e:   f04f 0300       mov.w   r3, #0  ; 0x0
  72:   607b            str     r3, [r7, #4]
  74:   e003            b.n     7e <main+0x76>
  76:   687b            ldr     r3, [r7, #4]
  78:   f103 0301       add.w   r3, r3, #1      ; 0x1
  7c:   607b            str     r3, [r7, #4]
  7e:   687a            ldr     r2, [r7, #4]
  80:   f248 639f       movw    r3, #34463      ; 0x869f
</code>

We will use //objcopy// to convert our //a.out// into a plain binary file:

  arm-none-eabi-objcopy -j .text a.out blink.bin -O binary

Before writing the code to flash, let's verify it using //octal dump//:

  od -t x1 blink.bin

Here is the output:

  0000000 00 10 00 20 09 00 00 00 80 b4 83 b0 00 af 4f f0
  0000020 00 03 7b 60 41 f2 18 03 c4 f2 02 03 4f f0 08 02
  0000040 1a 60 40 f6 04 42 c4 f2 01 02 44 f2 14 43 c4 f2
  0000060 44 43 13 60 40 f6 10 43 c4 f2 01 03 4f f4 00 72
  0000100 1a 60 4f f0 00 03 7b 60 03 e0 7b 68 03 f1 01 03
  0000120 7b 60 7a 68 48 f2 9f 63 c0 f2 01 03 9a 42 f4 d9
  0000140 40 f6 10 43 c4 f2 01 03 4f f0 00 72 1a 60 4f f0
  0000160 00 03 7b 60 03 e0 7b 68 03 f1 01 03 7b 60 7a 68
  0000200 48 f2 9f 63 c0 f2 01 03 9a 42 f4 d9 d2 e7 c0 46
  0000220

Let's look at the first four bytes - they represent 0x20001000 in little endian - these are the 4 bytes going to be stored at location 0x0 in the flash memory of the STM32 processor (to be used as the initial value for the stack pointer register).

The value of the next 4 bytes will be used by the Cortex processor to fetch the first instruction from memory (the address of the first instruction in main). The output from //objdump// tells us that the first instruction in //main// is at address 0x8 - but we are seeing 0x9 here - don't be surprised - that's the way it should be! The instruction set requires that the least significant bit of the jump address should be 1.

We can run through the rest of the //octal dump// and compare it with the machine code bytes in the .text section of the //objdump// - just to assure ourselves that nothing weird has occurred.

Now, we have to get the code into the flash memory of the processor using OpenOCD:

  $ telnet localhost 4444
  Trying 127.0.0.1...
  Connected to localhost.
  Escape character is '^]'.
  Open On-Chip Debugger

  > halt

  > stm32x unlock 0
  stm32x unlocked

  > flash erase_sector 0 0 0
  erased sectors 0 through 0 on flash bank 0 in 0.100966s

  
  > flash write_bank 0 /home/fosstronics/projects/stm32/mar1/blink.bin 0
  wrote  144 byte from file /home/fosstronics/projects/stm32/mar1/blink.bin to flash bank 0 at offset 0x00000000 in 1.510967s (0.093070 kb/s)

  > reset
  JTAG tap: stm32.cpu tap/device found: 0x3ba00477 (Manufacturer: 0x23b, Part: 0xba00, Version: 0x3)
  JTAG Tap/device matched
  JTAG tap: stm32.bs tap/device found: 0x16410041 (Manufacturer: 0x020, Part: 0x6410, Version: 0x1)
  JTAG Tap/device matched
  target state: halted
  target halted due to breakpoint, current mode: Thread 
  xPSR: 0x81000000 pc: 0x0000007c

If you don't see the red LED blinking after issuing the //reset// command, power cycle the primer.
