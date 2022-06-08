/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Main.c: C code entry.
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include <system.h>

unsigned char inportb (unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}
uint16_t inportw (unsigned short _port)
{
    uint16_t rv;
    __asm__ __volatile__ ("inw %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}
uint32_t inportl (unsigned short _port)
{
    uint32_t rv;
    __asm__ __volatile__ ("inl %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb (unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}
void outportw (unsigned short _port, uint16_t _data)
{
    __asm__ __volatile__ ("outw %1, %0" : : "dN" (_port), "a" (_data));
}
void outportl (unsigned short _port, uint32_t _data)
{
    __asm__ __volatile__ ("outl %1, %0" : : "dN" (_port), "a" (_data));
}
const uint32_t CPUID_FLAG_MSR = 1 << 5;
static int check_apic(void)
{
    unsigned int eax, unused, edx;
    __get_cpuid(1, &eax, &unused, &unused, &edx);
    return edx & CPUID_FEAT_EDX_APIC;
}
bool cpuHasMSR()
{
   uint32_t a, unused, d; // eax, edx
   __get_cpuid(1, &a,unused,unused, &d);
   return d & CPUID_FLAG_MSR;
}

void cpuGetMSR(uint32_t msr, uint32_t *lo, uint32_t *hi)
{
   asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

void cpuSetMSR(uint32_t msr, uint32_t lo, uint32_t hi)
{
   asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}
struct cpuid_t{
    uint8_t max;
    bool apic;
    bool msr;
};
struct cpuid_t cpuid;
char *fb;
int scanline;
int height;
extern char start[];
extern char end[];
extern int sectors_per_cluster;
extern int kbdisplay;
extern int csr_y;
extern struct file* opendir;
#define MAX(x,y) ((x>y)?x:y)
extern uint32_t exec_page_table[1024];
extern bool table_present[1024];
void main (multiboot_info_t* mbd, unsigned int magic)
{
    //init_video();
    kbdisplay = 0;
    fb = mbd->framebuffer_addr;
    scanline = mbd->framebuffer_pitch;
    height = mbd->framebuffer_height;
    psf_init();
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        puts("invalid magic");
        for(;;);
    }
    printf_("start:0x%x end:0x%x\n",start,end);
    cpuid.max = __get_cpuid_max(0,0);
    printf_("cpuid max:%d\n",cpuid.max);
    if(cpuid.max)
    {
        if (check_apic()) cpuid.apic = true; else cpuid.apic = false;
        if (cpuHasMSR()) cpuid.msr = true; else cpuid.msr = false;
    }

    acpi_init();
    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    //timer_install();
    keyboard_install();
    //dummy_install(

    __asm__ __volatile__ ("sti");

    int endmem;
    int i;
    for(i = 0; i < mbd->mmap_length;
        i += sizeof(multiboot_memory_map_t))
    {
        multiboot_memory_map_t* mmmt =
            (multiboot_memory_map_t*) (mbd->mmap_addr + i);

        //printf_("Start Addr: %x ",mmmt->addr);
        //printf_("| Length: %x ",  mmmt->len);
        //printf_("| Size: %x ",    mmmt->size);
        //printf_("| Type: %d \n",  mmmt->type);
        if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
            /*
             * Do something with this memory block!
             * BE WARNED that some of memory shown as availiable is actually
             * actively being used by the kernel! You'll need to take that
             * into account before writing to memory!
             */
             endmem = MAX(mmmt->len+mmmt->addr,endmem);
             valid_space(mmmt);
        }
    }
    alloc_install();
    paging_install(endmem);

    listmem();

    checkAllBuses();

    ATA_int_disable();
    uint8_t* target;
    int lba = 0x0;
    read_sectors_ATA_PIO(target,0, lba, 1);

    i = 0;
    /*while(i < 512)
    {
        printf_("%x",target[i]);
        i++;
    }*/
    fat_set(target);
    puts("loaded\n");
    fat_load_dir(root(),1);
    shell_init();
    kbdisplay = 1;
    int* val = kmalloc(sizeof(int)*0x1000);
    kfree(val);
    flush_tss();
    jump_usermode();
}
int errno = 0;
#define SYSCALL(a,b,c,ret) __asm__ ( "int $0x7f" :"=a"(ret) : "a" (a), "b"(b), "c"(c))
#define SYSCALL_MALLOC(size) SYSCALL(1,0,size,errno)
#define SYSCALL_MALLOC_ADDR(addr,size) SYSCALL(1,addr,size,errno)
void user()
{
    int ret;
    SYSCALL_MALLOC_ADDR(0x800000,2);
    for (;;);// shell_respond();
}
