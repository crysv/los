#include <system.h>
uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t kernel_page_table[1024] __attribute__((aligned(4096)));
uint32_t page_page_table[1024] __attribute__((aligned(4096)));
uint32_t framebuffer_table[1024] __attribute__((aligned(4096)));
bool table_present[1024] = {false};
extern void loadPageDirectory(unsigned int*);
extern void enablePaging();
extern char *fb;
extern int scanline;
extern int height;
struct page_addr
{
    uint32_t offset;
    uint32_t page;
    uint32_t table;
};
struct page_addr virt2page(uint32_t virt)
{
    uint32_t remainder = virt % 0x1000;
	uint32_t frame = virt / 0x1000;
	uint32_t table = frame / 1024;
	uint32_t idx = frame % 1024;
    struct page_addr address = {remainder,idx,table};
    return address;
}
#define invlpg(address) \
   __asm volatile ( "invlpg %0" : : "m" (*(char *)(address)) );
void dir_ent_inval(int index)
{
    for (int i = 0;i < 1024;i++)
         invlpg(index*(1024*0x1000)+i*0x1000);
}
/*void table_addr(int index, uint32_t addr)
{
    page_directory[index] &= 0xfff;
    page_directory[index] |= addr;
    dir_ent_inval(index);
}
void table_attr(int index, int attr)
{
    page_directory[index] &= ~(0xfff);
    page_directory[index] |= attr;
    dir_ent_inval(index);
}
void table_set(int index,void* addr,int attr)
{
    page_directory[index] = (unsigned int)addr | attr;
    dir_ent_inval(index);
}*/
void* page_addr(uint32_t phys,uint32_t addr,int attr,int size)
{
    addr&=~(0xfff);
    struct page_addr paddr = virt2page(addr);
    uint32_t adr = addr;
    uint32_t end = addr+(size*0x1000);
    printf_("page: addr:%x table:%x page:%d\n",adr,paddr.table,paddr.page);
    for (;addr < end;addr+=0x1000,phys+=0x1000)
    {
        struct page_addr page = virt2page(addr);
        printf_("table:%d",table_present[page.table]);
        //if (!table_present[page.table]) return 0;
        uint32_t table_value = page_directory[page.table];
        uint32_t* page_table = table_value & ~(0xfff);
        page_table[page.page] = (unsigned int)phys | attr;
        invlpg(addr);
    }
    //printf_("page: addr:%x table:%x page:%d\n",adr,paddr.table,paddr.page);
    return adr;
}
void paging_install(uint32_t endmem)
{
    //set each entry to not present
    unsigned int i;
    for(i = 0; i < 1024; i++)
    {
        // This sets the following flags to the pages:
        //   Supervisor: Only kernel-mode can access them
        //   Write Enabled: It can be both read from and written to
        //   Not Present: The page table is not present
        page_directory[i] = 0x00000002;
    }
    // holds the physical address where we want to start mapping these pages to.
    // in this case, we want to map these pages to the very beginning of memory.

    //we will fill all 1024 entries in the table, mapping 4 megabytes
    for (i = 0; i < 1024; i++)
    {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes ;)
        kernel_page_table[i] = (i * 0x1000) | 7; // attributes: supervisor level, read/write, present.
    }

    for (i = 0; i < 1024; i++)
    {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes ;)
        page_page_table[i] = 0x2; // attributes: supervisor level, read/write, not present.
    }
    struct page_addr fbp = virt2page(fb);
    for (i = 0; i < 1024; i++)
    {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes ;)
        framebuffer_table[i] = ((i * 0x1000)+(fbp.table*1024*0x1000)) | 3; // attributes: supervisor level, read/write, present.
    }
    struct page_addr endmemp = virt2page(endmem);
    // attributes: supervisor level, read/write, present
    page_directory[0] = ((unsigned int)kernel_page_table) | 7;
    page_directory[1] = ((unsigned int)page_page_table) | 3;
    page_directory[fbp.table] = ((unsigned int)framebuffer_table) | 3;
    table_present[0] = true;
    table_present[1] = true;
    table_present[fbp.table] = true;
    puts("paging enable:\n");
    loadPageDirectory(page_directory);
    enablePaging();
    uint32_t* alloc_tables = kmalloc(sizeof(uint32_t)*1024*endmemp.table);
    for (i = 2; i < endmemp.table-1;i++)
    {
        if (i != table_present[i]==false)
        {
            uint32_t* currtable = &alloc_tables[(i-2)*1024];
            for (i = 0; i < 1024; i++)
            {
                // As the address is page aligned, it will always leave 12 bits zeroed.
                // Those bits are used by the attributes ;)
                currtable[i] = 0x2; // attributes: supervisor level, read/write, not present.
            }
            page_directory[i] = ((unsigned int)currtable) | 3;
            table_present[i] = true;
        }
    }
    loadPageDirectory(page_directory);
}
