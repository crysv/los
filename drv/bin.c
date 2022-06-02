#include <system.h>
extern sectors_per_cluster;
//extern void puts();
struct intStrMap
{
      void* addr;
      char* name;
};
struct intStrMap;
#define MAX_FUNCS 0xff
struct intStrMap func[MAX_FUNCS];
#define SETFUNC(i,a,b) func[i].addr = a;func[i].name = b
void setfuncs()
{
    //implicit declaration
    puts("");
    SETFUNC(0,puts,"puts");
}
void* __attribute__((stdcall)) symbol(char* name)
{
    for (int i = 0;i<MAX_FUNCS;i++)
    {
        if(strcmp(func[i].name,name)==0)
        {
            outportw(0x8A00,0x8A00); outportw(0x8A00,0x08AE0);
            return func[i].addr;
        }
    }
    return 0;
}
void binexec(int clus)
{
    uint8_t volatile * volatile target = kmalloc(512*sectors_per_cluster);
    read_sectors_ATA_PIO(target,0, cluster2sector(clus), sectors_per_cluster);
    outportw(0x8A00,0x8A00); outportw(0x8A00,0x08AE0);
    int (*entry)(uint32_t* a)__attribute__((fastcall)) = target;
    setfuncs();
    uint32_t* a = symbol;
    entry(a);
    kfree (target);
}
