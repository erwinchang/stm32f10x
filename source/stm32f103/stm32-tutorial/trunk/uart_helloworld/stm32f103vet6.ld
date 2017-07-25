/*************************************************/ 
/* filename: stm32f103VET6.ld                    */ 
/* linkscript for STM32F103VET6  microcontroller */ 
/*                                               */ 
/*************************************************/  
MEMORY 
{
  /*FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512k*/
  /*Let's use the address space start from 0x00000000*/
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 512k 
  SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 64k 
}

/* Section Definitions */ 
SECTIONS
{
  .isr_vector :
  {
    KEEP(*(.isr_vector))
    isr.o(.text)
    . = ALIGN(4);
    _eisr_vector = .;
  }

  .text : AT (_eisr_vector)
  {
    _text = .;
    *(EXCLUDE_FILE(isr.o) .text)
    *(.rodata)
    . = ALIGN(4);
    _etext = .;
  } > SRAM

  .data :
  { 
    _data = .;
    *(.data)
    . = ALIGN(4);
    _edata = . ;
  } > SRAM

  /* .bss section which is used for uninitialized data */
  .bss (NOLOAD) :
  {
    _bss = . ;
    *(.bss)
    . = ALIGN(4);
    _ebss = . ;
  } > SRAM

  _end = . ;
}
