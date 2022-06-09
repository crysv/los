#include <fat.h>
#include <system.h>
int fat_size;
int sector_size;
int first_fat_sector;
int reserved_sectors;
int root_dir_sectors;
int first_data_sector;
int first_root_dir_sector;
int sectors_per_cluster;
int fat_type;
int total_clusters;
int data_sectors;
int total_sectors;
int fat_ready;
struct file
{
    char fullname[12];char null;
    char name[8];char null2;
    char ext[3];char null3;
    int cluster;
    bool directory;
};
int table_val(int active_cluster)
{
    unsigned char FAT_table[sector_size];
    unsigned int fat_offset = active_cluster * 2;
    unsigned int fat_sector = first_fat_sector + (fat_offset / sector_size);
    unsigned int ent_offset = fat_offset % sector_size;

    //at this point you need to read from sector "fat_sector" on the disk into "FAT_table".

    read_sectors_ATA_PIO(FAT_table,0, fat_sector, 1);

    return *(unsigned short*)&FAT_table[ent_offset];
}
int cluster2sector(int active_cluster)
{
   return  ((active_cluster - 2) * sectors_per_cluster) + first_data_sector;
}
int root()
{
    return first_root_dir_sector;
}
uint32_t* fat_list(int sector);
void fat_set(struct fat_BS* fat_boot)
{
    fat_ready = 0;
    sector_size = fat_boot->bytes_per_sector;
    struct fat_extBS_32 *fat_boot_ext_32 = fat_boot+0x024;
    fat_size = (fat_boot->table_size_16 == 0)? fat_boot_ext_32->table_size_32 : fat_boot->table_size_16;
    first_fat_sector = fat_boot->reserved_sector_count;
    reserved_sectors = fat_boot->reserved_sector_count;
    total_sectors = (fat_boot->total_sectors_16 == 0)? fat_boot->total_sectors_32 : fat_boot->total_sectors_16;
    root_dir_sectors = ((fat_boot->root_entry_count * 32) + (fat_boot->bytes_per_sector - 1)) / fat_boot->bytes_per_sector;
    first_data_sector = fat_boot->reserved_sector_count + (fat_boot->table_count * fat_size) + root_dir_sectors;
    data_sectors = total_sectors - (fat_boot->reserved_sector_count + (fat_boot->table_count * fat_size) + root_dir_sectors);
    first_root_dir_sector = fat_boot->reserved_sector_count + (fat_boot->table_count * fat_size);
    total_clusters = data_sectors / fat_boot->sectors_per_cluster;
    sectors_per_cluster = fat_boot->sectors_per_cluster;
    if (sector_size == 0)
    {
        fat_type = 0;
    }
    else if(total_clusters < 4085)
    {
        fat_type = 12;
    }
    else if(total_clusters < 65525)
    {
        fat_type = 16;
    }
    else
    {
        fat_type = 32;
    }
    fat_ready = 1;
    //fat_list(first_root_dir_sector);
    //uint8_t* target;
    //read_sectors_ATA_PIO(target,0, 3, 1);
    //printf_("%s",target);
}
struct file openfile;
struct file* fat_dir_index(int sector,int i)
{
    uint8_t* target;
    read_sectors_ATA_PIO(target,0, sector, sectors_per_cluster);
    openfile.null = openfile.null2 = openfile.null3 = 0x0;
    //if (((uint16_t)target[(i*0x20)+0x1B] << 8) | target[(i*0x20)+0x1A]==0) continue;
    //printf_("\n%s cluster:%x",&target[i*0x40+0x20],((uint16_t)target[(i*0x40+0x20)+0x1B] << 8) | target[(i*0x40+0x20)+0x1A]);
    //if (((uint16_t)target[(i*0x20)+0x1B] << 8) | target[(i*0x20)+0x1A]!=0) cluster_chain(((uint16_t)target[(i*0x20)+0x1B] << 8) | target[(i*0x20)+0x1A]);
    openfile.cluster = ((uint16_t)target[(i*0x40+0x20)+0x1B] << 8) | target[(i*0x40+0x20)+0x1A];
    openfile.directory = (target[(i*0x40+0x20)+0xb]&0x10);
    for(int e = 0;e < 8;e++) openfile.name[e] = target[(i*0x40+0x20)+e];
    for(int e = 8;e < 11;e++) openfile.ext[e-8] = target[(i*0x40+0x20)+e];
    //(target[i*0x40+0x20]==0x0)
    return &openfile;
}
struct file opendir[32];
int fat_dir_find(int sector,char name[11])
{
    if (fat_type==16){
        uint8_t* target;
        read_sectors_ATA_PIO(target,0, sector, sectors_per_cluster);
        for(int i = 0;;i++)
        {
            if (target[i*0x40+0x20]==0x0) break;
            for(int e = 0;e < 11;e++)
            {
                if (target[(i*0x40+0x20)+e]!=name[e])
                    break;
                if (e == 10)
                    return ((uint16_t)target[(i*0x40+0x20)+0x1B] << 8) | target[(i*0x40+0x20)+0x1A];
            }
        }
    }
    else{ puts("fat16 only");return; }
}
int fat_load_dir(int sector,int print)
{
   if (fat_type==16){
        uint8_t* target;
        read_sectors_ATA_PIO(target,0, sector, sectors_per_cluster);
        int i;
        i = 0;
        /*while(i < 512)
        {
            printf_("%x",target[i]);
            i++;
        }*/
        for(int i = 0;;i++)
        {
            opendir[i].null = opendir[i].null2 = opendir[i].null3 = 0x0;
            if (target[i*0x40+0x20]==0x0) break;
            //if (((uint16_t)target[(i*0x20)+0x1B] << 8) | target[(i*0x20)+0x1A]==0) continue;
            //printf_("\n%s cluster:%x",&target[i*0x40+0x20],((uint16_t)target[(i*0x40+0x20)+0x1B] << 8) | target[(i*0x40+0x20)+0x1A]);
            //if (((uint16_t)target[(i*0x20)+0x1B] << 8) | target[(i*0x20)+0x1A]!=0) cluster_chain(((uint16_t)target[(i*0x20)+0x1B] << 8) | target[(i*0x20)+0x1A]);
            opendir[i].cluster = ((uint16_t)target[(i*0x40+0x20)+0x1B] << 8) | target[(i*0x40+0x20)+0x1A];
            openfile.directory = (target[(i*0x40+0x20)+0xb]&0x10);
            for(int e = 0;e < 8;e++) opendir[i].name[e] = target[(i*0x40+0x20)+e];
            for(int e = 8;e < 11;e++) opendir[i].ext[e-8] = target[(i*0x40+0x20)+e];
            printf_("name:%s ext:%s cluster:%x(%d)\n",opendir[i].name,opendir[i].ext,opendir[i].cluster,opendir[i].cluster);
        }

    }
    else{ puts("fat16 only");return; }
}
