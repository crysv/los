#include <system.h>
uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t kernel_page_table[1024] __attribute__((aligned(4096)));
uint32_t page_page_table[1024] __attribute__((aligned(4096)));
uint32_t framebuffer_table[1024] __attribute__((aligned(4096)));
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
uint32_t* currpdir;
void* table_create(int i)
{
    uint32_t* table = kmalloc(sizeof(uint32_t)*1024);
    memset(table,0,sizeof(uint32_t)*1024);
    for (int j = 0; j < 1024; j++)
    {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes ;)
        table[j] = 0x2; // attributes: supervisor level, read/write, not present.
    }
    currpdir[i] = ((unsigned int)table) | 7;
    return table;
}
void* page_addr(uint32_t phys,uint32_t addr,int attr,int size)
{
    addr&=~(0xfff);
    phys&=~(0xfff);
    struct page_addr paddr = virt2page(addr);
    uint32_t adr = addr;
    uint32_t end = addr+(size*0x1000);
    //printf_("page: paddr:%x table:%x page:%d ",phys,table_present[paddr.table],paddr.page);
    //printf_("table_state:%x ",table_present[paddr.table]);
    for (;addr < end;addr+=0x1000,phys+=0x1000)
    {
        struct page_addr page = virt2page(addr);
        uint32_t table_value = currpdir[page.table];
        if (!(table_value&1)) table_value = table_create(page.table);
        uint32_t* page_table = table_value & ~(0xfff);
        page_table[page.page] = (unsigned int)phys | attr;
        invlpg(addr);
    }
    //printf_("page: addr:%x table:%x page:%d\n",adr,paddr.table,paddr.page);
    return adr;
}
uint32_t* createpdir(uint32_t* pdir)
{
    unsigned int i;
    for(i = 0; i < 1024; i++)
        pdir[i] = 0x00000002;
    struct page_addr fbp = virt2page(fb);
    pdir[0] = ((unsigned int)kernel_page_table) | 3;
    pdir[1] = ((unsigned int)page_page_table) | 3;
    pdir[fbp.table] = ((unsigned int)framebuffer_table) | 3;
}
void paging_install()
{
    unsigned int i;
    for(i = 0; i < 1024; i++)
    {
        page_directory[i] = 0x00000002;
    }
    for (i = 0; i < 1024; i++)
    {
        kernel_page_table[i] = (i * 0x1000) | 3;
    }
    for (i = 0; i < 1024; i++)
    {
        page_page_table[i] = 0x2;
    }
    struct page_addr fbp = virt2page(fb);
    for (i = 0; i < 1024; i++)
    {
        framebuffer_table[i] = ((i * 0x1000)+(fbp.table*1024*0x1000)) | 3;
    }
    page_directory[0] = ((unsigned int)kernel_page_table) | 3;
    page_directory[1] = ((unsigned int)page_page_table) | 3;
    page_directory[fbp.table] = ((unsigned int)framebuffer_table) | 3;
    puts("paging enable:\n");
    loadPageDirectory(page_directory);
    enablePaging();
}
