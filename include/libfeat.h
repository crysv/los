extern void puts(char* text);
extern void* kmalloc(int size);
extern void kfree(void* addr);
char* libflist[] =
{
    "puts",
    "kmalloc",
    "kfree"
};
char* libfdeflist[] =
{
    "void puts(char* text);",
    "void* kmalloc(int size);",
    "void kfree(void* addr);"
};
int libflsize = 3;
