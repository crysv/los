OUTPUT_FORMAT("binary")
SECTIONS
{
    .text :
    {
        *(.*)
    }
    end = .;
}
