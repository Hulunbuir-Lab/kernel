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

  acpiAddress = .;
  . += 8;
  memMapAddress = .;
  . += 8;
  memMapSize = .;
  . += 8;
  memMapDescriptorSize = .;
  /DISCARD/ :
  {
    *(.comment)
  }
}
