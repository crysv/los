typedef __UINT8_TYPE__ uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __UINT64_TYPE__ uint64_t;
extern int main();
void* malloc(int size);
void free(void* addr);
void outportw (unsigned short _port, uint16_t _data)
{
    __asm__ __volatile__ ("outw %1, %0" : : "dN" (_port), "a" (_data));
}
int start()
{
    int retval;
    __asm__ __volatile__ ("call main" : "=a"(retval));
    exit(retval);
}
void debug_call()
{
    outportw(0x8A00,0x8A00); outportw(0x8A00,0x08AE0);
}
