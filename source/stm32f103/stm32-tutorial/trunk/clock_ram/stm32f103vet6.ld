/****************************************************/ 
/* filename: stm32f103VET6.ld                       */ 
/* linkscript for STM32F103VET6  microcontroller    */ 
/*                                                  */
/* We have to load the sections to RAM by OpenJTAG  */ 
/****************************************************/
  
MEMORY 
{
  /*FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512k*/
  /*Let's use the address space start from 0x00000000*/
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 512k 
  SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 64k 
}

hal_startup_stack = 0x20000000 + 1024*64;

ENTRY(reset_vector)

/* Section Definitions */ 
SECTIONS
{
  .vectors :
  {
    *(.vectors)
    . = ALIGN(4);
  } > SRAM

  .text : 
  {
    *(.text)
    *(.rodata)
    . = ALIGN(4);
  } > SRAM

  .data :
  { 
    *(.data)
    . = ALIGN(4);
  } > SRAM

  /* .bss section which is used for uninitialized data */
  .bss (NOLOAD) :
  {
    *(.bss)
    . = ALIGN(4);
  } > SRAM
}
