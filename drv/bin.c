#include <system.h>
extern sectors_per_cluster;
void* binload(int clus,uint32_t addr)
{
    uint8_t volatile * volatile target = malloc_addr(addr,512*sectors_per_cluster);
    read_sectors_ATA_PIO(target,0, cluster2sector(clus), sectors_per_cluster);
    int (*entry)() = target;
    return entry;//entry();
}
void* binloadpic(int clus)
{
    uint8_t volatile * volatile target = malloc(512*sectors_per_cluster);
    read_sectors_ATA_PIO(target,0, cluster2sector(clus), sectors_per_cluster);
    int (*entry)() = target;
    return entry;//entry();
}
