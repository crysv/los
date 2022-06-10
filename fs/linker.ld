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
        EXCLUDE_FILE(*lib.o) *(.*)
        main = main;
    }
    lib = .;
    .lib :
    {
        *(.*)
    }
}
