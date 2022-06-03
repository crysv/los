#include <system.h>
#define BLOCKSIZE 0x1000
#define BIT(shift) (1<<shift)
#define FREE_ALLOC 0x0
#define SINGLE_ALLOC 0x1
#define RESERVED 0xff
#define ERR_NOMEM 0x0
uint32_t hmem;
extern char end[];
uint8_t* bitmap;
uint8_t* mem;
uint32_t blocks;
void valid_space(multiboot_memory_map_t* mem)
{
    if(mem->addr = 0x10000)
    {
        hmem = mem->len;
    }
}

void alloc_install()
{
    bitmap = end;
    blocks = hmem/BLOCKSIZE;
    for(int i = 0;i<blocks;i++)
    {
        bitmap[i] = 0x0;
    }
    mem = &bitmap[blocks];
}

void listmem()
{
    printf_("hmem:%x ",hmem);
    printf_("blocks:%x ",hmem/BLOCKSIZE);
    printf_("bitmap:%x ",bitmap);
    printf_("mem:%x\n",mem);

}
void kfree(void* addr)
{
    if (!addr) return;
    if (addr < mem) {puts("kfree: address below range\n");return;}
    int index = ((int)addr-(int)mem)/BLOCKSIZE;
    if (index > blocks) {puts("kfree: address not in range\n");return;}
#ifdef ALLOC_DEBUG
    printf_("kfree: index:%x",index);
    printf_(" addr:%x",mem + index*BLOCKSIZE);
#endif
    if (bitmap[index]==0x1) {
#ifdef ALLOC_DEBUG
        puts(" size:1\n");
#endif
        bitmap[index] = FREE_ALLOC;
    }
}
void* kmalloc_resv(int addr,int size)
{
    if (addr&(BLOCKSIZE-1))
    {
        addr&=~(BLOCKSIZE-1);
        addr+=BLOCKSIZE;
    }
    int end = addr + size;
    if (end&(BLOCKSIZE-1))
    {
        end&=~(BLOCKSIZE-1);
        end+=BLOCKSIZE;
    }

    int index = ((int)addr-(int)mem)/BLOCKSIZE;
    int oldaddr = addr;
#ifdef ALLOC_DEBUG
    int oldindex = index;
#endif
    for (;addr <= end;addr+=BLOCKSIZE)
        bitmap[index++] = RESERVED;
#ifdef ALLOC_DEBUG
    printf_("kmalloc_resv: index:%x",oldindex);
    printf_(" addr:%x",oldaddr);
    printf_(" size:%x\n",((end-addr)/BLOCKSIZE)+1);
#endif
    return oldaddr;
}
void* kmalloc(int size)
{
    if (!size) return 0x0;
    int request;
    if (size&(BLOCKSIZE-1))
    {
        size&=~(BLOCKSIZE-1);
        size+=BLOCKSIZE;
    }
    request = size>>12;
    if (request==1)
    {
        for(int i = 0;i<blocks;i++)
        {
            if(bitmap[i] == FREE_ALLOC)
            {
#ifdef ALLOC_DEBUG
                printf_("kmalloc: index:%x",i);
                printf_(" addr:%x",mem + i*BLOCKSIZE);
                puts(" size:1\n");
#endif
                bitmap[i] = SINGLE_ALLOC;
                return mem + i*BLOCKSIZE;
            }
        }
        puts("kmalloc: no memory\n");
        return ERR_NOMEM;
    }
    else puts("kmalloc: greater than 1 block\n");
    /*{
        for(int i = 0;i<blocks;i++)
        {
            if (bitmap[i]==0)
            {
                int old = i;
                int oldold = i;
                int count = 0;
                for(;i<blocks;i++)
                {
                    if (bitmap[i]!=0) break;
                    if (count == request)
                    {
                        for(;old<i+1;old++)
                        {
                            bitmap[old] = 0x1;
                        }
                        return bitmap + oldold*BLOCKSIZE;
                    }
                    count++;
                }
            }
        }
    }*/
}
