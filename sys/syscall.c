#include <system.h>
#define EAXR(a) __asm__ ( "movl %%eax, %0": "=a" (a))
#define EBXR(b) __asm__ ( "movl %%ebx, %0": "=b" (b))
#define ECXR(c) __asm__ ( "movl %%ecx, %0": "=c" (c))
#define EDXR(d) __asm__ ( "movl %%edx, %0": "=d" (d))
typedef uint32_t (*syscall_t)(uint32_t b,uint32_t c,uint32_t d);
uint32_t malloc_call(uint32_t b,uint32_t c,uint32_t d)
{
    return kmalloc_addr((void*)b,(int)c);
}
syscall_t calltable[] =
{
    NULL,
    malloc_call
};
uint32_t _syscall()
{
    int a, b, c, d;
    EAXR(a);
    EBXR(b);
    ECXR(c);
    EDXR(d);
    printf_("eax:%x ebx:%x",a,b);
    return calltable[a](b,c,d);
}
