OUTPUT_FORMAT("binary")
ENTRY(start)
phys = 0x200000;
SECTIONS
{
    . = 0x200000;
    .text.start :
    {
        *(.text.start)
    }
    .text :
    {
        *(.*)
    }
}
