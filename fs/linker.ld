OUTPUT_FORMAT("binary")
ENTRY(start)
phys = 0x8000000;
SECTIONS
{
    . = 0x8000000;
    .text.start :
    {
        *(.text.start)
    }
    .text :
    {
        *(.*)
        main = main;
    }
    . = ALIGN(4096);
    libbase = .;
    . += 4096;
    libgbase = .;
}
