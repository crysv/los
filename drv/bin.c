#include <system.h>
extern sectors_per_cluster;
#include <libfeat.h>
//extern void puts();
struct vptrStrMap
{
      void* addr;
      char* name;
};
struct vptrStrMap;
#define MAX_FUNCS 0xff
struct vptrStrMap func[MAX_FUNCS];
#define SETFUNC(i,a,b) func[i].addr = a;func[i].name = b
void setfuncs()
{
    SETFUNC(0,puts,"puts");
    SETFUNC(1,kmalloc,"kmalloc");
    SETFUNC(2,kfree,"kfree");
}
void* __attribute__((stdcall)) symbol(char* name)
{
    for (int i = 0;i<MAX_FUNCS;i++)
    {
        if(strcmp(func[i].name,name)==0)
        {
            return func[i].addr;
        }
    }
    return 0;
}
/*void __attribute__((stdcall)) resolve(void* ptr,char* name)
{
    *ptr = symbol(name);
}*/
void binexec(int clus)
{
    uint8_t volatile * volatile target = kmalloc_addr(0x400000,512*sectors_per_cluster);
    read_sectors_ATA_PIO(target,0, cluster2sector(clus), sectors_per_cluster);
    int (*entry)(uint32_t* a)__attribute__((fastcall)) = target;
    setfuncs();
    uint32_t* a = symbol;
    entry(a);
}
