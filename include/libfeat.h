extern void puts(char* text);
extern void* kmalloc(int size);
extern void kfree(void* addr);
char* libflist[] =
{
    "puts",
    "kmalloc",
    "kfree"
};
int libflsize = 3;
