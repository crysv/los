OUTPUT_FORMAT("binary")
ENTRY(start)
phys = 0x400000;
SECTIONS
{
    . = 0x400000;
    .text.start :
    {
        *(.text.start)
    }
    .text :
    {
        *(.*)
        main = main;
    }
}
