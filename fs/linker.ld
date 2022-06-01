OUTPUT_FORMAT("binary")
ENTRY(start)
phys = 0x00100000;
SECTIONS
{
    . = 2M;
    .text :
    {
        *(.*)
    }
}
