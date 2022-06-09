int _dummy;
#define SYSCALL(a,b,c,ret) __asm__ __volatile__ ( "int $0x7f" :"=a"(ret) : "a" (a), "b"(b), "c"(c))
#define SYSCALL_MALLOC(size,_ret) SYSCALL(1,0,size,_ret)
#define SYSCALL_MALLOC_ADDR(addr,size,_ret) SYSCALL(1,addr,size,_ret)
#define SYSCALL_FREE(addr) SYSCALL(2,addr,0,_dummy)
#define SYSCALL_EXIT(code) SYSCALL(3,code,0,_dummy)
#define CALLWRAP(type,macro) { type _ret; macro ;return _ret;}
#define VCALLWRAP(macro) { macro ;}
void* malloc(int size) CALLWRAP(void*,SYSCALL_MALLOC(size,_ret))
void free(void* addr)  VCALLWRAP(SYSCALL_FREE(addr))
void exit(int code)    VCALLWRAP(SYSCALL_EXIT(code))
