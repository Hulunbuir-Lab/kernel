OUTPUT_ARCH( "loongarch" )
ENTRY( _start )

SECTIONS
{
  . = 0xA04000;

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

  KernelEnd = .
}
