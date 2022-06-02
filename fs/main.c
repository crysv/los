typedef __UINT8_TYPE__ uint8_t;
typedef __UINT32_TYPE__ uint32_t;
int __attribute__((fastcall)) start(uint32_t* a)
{
	//a[];
	void* (*symbol)(char* name)__attribute__((stdcall)) = a;
	char name[] = "puts\0";
	void (*puts)(char* str) = symbol(name);
	char str[] = "yes\0";
	puts(str);
	return 0xdead;
}
//void _GLOBAL_OFFSET_TABLE_()
