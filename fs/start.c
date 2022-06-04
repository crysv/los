typedef __UINT8_TYPE__ uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __UINT64_TYPE__ uint64_t;
extern int main();
void (*puts)(char* str);
void* (*kmalloc)(int size);
void (*kfree)(void* addr);
void outportw (unsigned short _port, uint16_t _data)
{
    __asm__ __volatile__ ("outw %1, %0" : : "dN" (_port), "a" (_data));
}
int __attribute__((fastcall)) start(uint32_t* a)
{
	void* (*symbol)(char* name)__attribute__((stdcall)) = a;
	puts = symbol("puts");
	kmalloc = symbol("kmalloc");
	kfree = symbol("kfree");
	outportw(0x8A00,0x8A00); outportw(0x8A00,0x08AE0);
	__asm__ __volatile__ ("call main");
}
void some_call()
{

}
