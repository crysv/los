#include <system.h>
extern sectors_per_cluster;
void binexec(int clus)
{
    uint8_t volatile * volatile target = kmalloc(512*sectors_per_cluster);
    read_sectors_ATA_PIO(target,0, cluster2sector(clus), sectors_per_cluster);
    //outportw(0x8A00,0x8A00); outportw(0x8A00,0x08AE0);
    //printf_("target:%x\n",target);
    int (*entry)(int a)__attribute__((fastcall)) = target;
    entry(0);
}
