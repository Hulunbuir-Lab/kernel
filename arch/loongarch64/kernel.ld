OUTPUT_ARCH( "loongarch" )
ENTRY( kernelMain )

SECTIONS
{
  . = 0x200000;

  .text : {
    *(.text)
  }

  .data : {
    *(.data)
  }

  .bss : {
    *(.bss)
  }

  .rodata : {
    *(.rodata)
  }

  .stack : {
    *(.stack)
  }

  /DISCARD/ :
  {
    *(.comment)
  }
}
