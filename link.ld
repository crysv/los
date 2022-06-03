ENTRY(start)
phys = 0x00100000;
SECTIONS
{
  start = .;
  .text phys : AT(phys) {
    code = .;
    /* link the multiboot struct here */
    . = ALIGN(8);
    KEEP(*(.multiboot))
    *(.text)
    *(.rodata)
    . = ALIGN(4096);
  }
  .data : AT(phys + (data - code))
  {
    data = .;
    *(.data)
    . = ALIGN(4096);
  }
  .unicode : AT (phys + (unicode - code))
  {
    unicode_buffer = .;
    . += 0x20000;
    . = ALIGN(4096);
  }
  .bss : AT(phys + (bss - code))
  {
    bss = .;
    *(.bss)
    . = ALIGN(4096);
  }
  end = .;
}
