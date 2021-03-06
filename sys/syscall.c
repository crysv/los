#include <system.h>
typedef struct {
    uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
} Registers;
typedef struct Task {
    Registers regs;
    uint32_t* buffer;
    int idx;
    bool active;
    struct Task *next;
} __attribute__((packed)) Task;
extern Task *currtask;
typedef uint32_t (*syscall_t)(uint32_t b,uint32_t c,uint32_t d);
#define CALLWRAP(callwrap,callreal) uint32_t callwrap (uint32_t b,uint32_t c,uint32_t d) { callreal ;}
CALLWRAP(malloc_call,kmalloc_addr((void*)b,(int)c))
CALLWRAP(free_call,kfree((void*)b))
CALLWRAP(exit_call,freetask(currtask);nexttask())
extern uint32_t init(uint32_t b,uint32_t c,uint32_t d);
syscall_t calltable[] =
{
    init,
    malloc_call,
    free_call,
    exit_call
};
uint32_t _syscall(uint32_t a,uint32_t b,uint32_t c,uint32_t d)
{
    uint32_t ret = calltable[a](b,c,d);
    //printf_("eax:0x%08x ebx:0x%08x ecx:0x%08x edx:0x%08x ret:0x%08x\n",a,b,c,d,ret);
    return ret;
}
