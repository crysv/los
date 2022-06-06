#include <system.h>
uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t kernel_page_table[1024] __attribute__((aligned(4096)));
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
void paging_install()
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
        kernel_page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
    }
    struct page_addr fbp = virt2page(fb);
    for (i = 0; i < 1024; i++)
    {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes ;)
        framebuffer_table[i] = ((i * 0x1000)+(fbp.table*1024*0x1000)) | 3; // attributes: supervisor level, read/write, present.
    }
    // attributes: supervisor level, read/write, present
    page_directory[0] = ((unsigned int)kernel_page_table) | 3;
    page_directory[fbp.table] = ((unsigned int)framebuffer_table) | 3;
    puts("paging");
    loadPageDirectory(page_directory);
    enablePaging();
}
