typedef __UINT8_TYPE__ uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __UINT64_TYPE__ uint64_t;
extern int main();
void outportw (unsigned short _port, uint16_t _data)
{
    __asm__ __volatile__ ("outw %1, %0" : : "dN" (_port), "a" (_data));
}
#define VSYSCALL(a,b,c,d) __asm__ __volatile__ ( "int $0x7f" : : "a" (a), "b"(b), "c"(c), "d"(d))
#define YEILD __asm__ __volatile__ ( "int $0x7e" : : )
extern char libbase[];
volatile uint32_t _buf[32];
int start()
{
    VSYSCALL(0,0,_buf,0);
    _buf[0] = "LIB     BIN";
    _buf[1] = libbase;
    _buf[2] = 0x0;
    _buf[3] = 0x0;
    VSYSCALL(0,1,0,0);
    int retval;
    YEILD;
    __asm__ __volatile__ ("call main" : "=a"(retval));
    exit(retval);
}
void debug_call()
{
    outportw(0x8A00,0x8A00); outportw(0x8A00,0x08AE0);
}
