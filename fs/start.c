typedef __UINT8_TYPE__ uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __UINT64_TYPE__ uint64_t;
extern int main();
void outportw (unsigned short _port, uint16_t _data)
{
    __asm__ __volatile__ ("outw %1, %0" : : "dN" (_port), "a" (_data));
}
#define SYSCALL(a,b,c,ret) __asm__ __volatile__ ( "int $0x7f" :"=a"(ret) : "a" (a), "b"(b), "c"(c))
int start()
{
    uint8_t* _ret;
    SYSCALL(0,0,0,_ret);
    _ret[0x0];
    int retval;
    __asm__ __volatile__ ("call main" : "=a"(retval));
    exit(retval);
}
void debug_call()
{
    outportw(0x8A00,0x8A00); outportw(0x8A00,0x08AE0);
}
