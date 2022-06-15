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
typedef struct {
    uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
} Registers;
typedef struct Task {
    Registers regs;
    uint32_t* buffer;
    int idx;
    bool active;
    struct Task *next;
} __attribute__((packed)) Task;
struct cpuid_t{
    uint8_t max;
    bool apic;
    bool msr;
};
struct cpuid_t cpuid;
char *fb;
int scanline;
int height;
int taskidx;
Task *tasklist[32];
Task *currtask;
extern char start[];
extern char end[];
extern int sectors_per_cluster;
extern int kbdisplay;
extern int csr_y;
extern struct file* opendir;
#define MAX(x,y) ((x>y)?x:y)
extern uint32_t page_directory[1024];
extern bool table_present[1024];
extern uint32_t* currpdir;
uint32_t eflags_read()
{
    uint32_t a;
    __asm__ __volatile__ ( "pushf ; pop %%eax" :"=a"(a));
    return a;
}
#define LOADTASK(task)\
currtask = task ; \
currpdir = task ->regs.cr3; \
loadPageDirectory( task ->regs.cr3);\
__asm__ __volatile__ (\
"pushl %%eax; \
 movw $0x23, %%ax; \
 movw %%ax, %%ds; \
 movw %%ax, %%es; \
 movw %%ax, %%fs; \
 movw %%ax, %%gs; \
 popl %%eax; \
 pushl $0x23; \
 pushl %0; \
 pushf;\
 pushl $0x1b; \
 pushl %1; \
 mov %2, %%ecx;mov %3, %%edx; \
 mov %4, %%esi;mov %5, %%edi;mov %6, %%ebp; \
 mov %7, %%ebx;mov %8, %%eax; \ \
 iret;" : : \
"g"( task ->regs.esp),\
"g"( task ->regs.eip),\
"g"( task ->regs.ecx),\
"g"( task ->regs.edx),\
"g"( task ->regs.esi),\
"g"( task ->regs.edi),\
"g"( task ->regs.ebp),\
"g"( task ->regs.ebx),\
"g"( task ->regs.eax));
#define NEWTASK(idx,func)\
pdir = kmalloc(sizeof(uint32_t)*1024); \
createpdir(pdir); \
currpdir = pdir; \
loadPageDirectory(pdir); \
createTask(kmalloc(sizeof(Task)), idx , func ,eflags_read(),pdir);
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
    currpdir = page_directory;
    paging_install();

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
    memset(tasklist,0,sizeof(tasklist));
    taskidx = -1;
    createTask(currtask,0,0,page_directory);
    flush_tss();
    uint32_t* pdir;
    NEWTASK(0,binload(fat_dir_find(root(),"MAIN    BIN"),0x8000000));
    nexttask();
    //jump_usermode(task->regs.eip,task->regs.esp);
}
void nexttask()
{
    int taskidxo = taskidx;
    do
    {
        taskidx++; if (taskidx==32) taskidx = 0;
        if (taskidxo == taskidx) {puts("\ntasklist empty");for(;;);}
    }
    while (!tasklist[taskidx]||!tasklist[taskidx]->active);
    LOADTASK(tasklist[taskidx]);
}
void savetask(uint32_t cr3,uint32_t eax,uint32_t ebx,uint32_t ecx,
              uint32_t edx,uint32_t esi,uint32_t edi,uint32_t ebp,
              uint32_t eip,uint32_t eflags,uint32_t esp)
{
    currtask->regs.eax = eax;
    currtask->regs.ebx = ebx;
    currtask->regs.ecx = ecx;
    currtask->regs.edx = edx;
    currtask->regs.esi = esi;
    currtask->regs.edi = edi;
    currtask->regs.esp = esp;
    currtask->regs.ebp = ebp;
    currtask->regs.eip = eip;
    currtask->regs.eflags = eflags;
    currtask->regs.cr3 = cr3;
}
void freetask(Task *task)
{
    currpdir = page_directory;
    kfree(task->regs.cr3);
    tasklist[task->idx] = 0;
}
void createTask(Task *task,int idx,void (*main)(), uint32_t flags, uint32_t *pagedir) {
    memset(task,0,sizeof(Task));
    task->regs.eflags = flags;
    task->regs.eip    = (uint32_t) main;
    task->regs.cr3    = (uint32_t) pagedir;
    uint32_t stack    = malloc(0x4000);
    task->regs.esp    = (uint32_t) stack+0x3ff0;
    task->regs.ebp    = 0xffffffff;
    task->active      = true;
    task->idx         = idx;
    tasklist[idx]     = task;
}

uint32_t loadlibs()
{
    for(size_t i = 0; i < 32; i+=2) {
        if (!currtask->buffer[i]||!currtask->buffer[i+1]) return 0;
        char name[12];
        memcpy(name,(char*)currtask->buffer[i],11);
        name[12] = 0x0;
        int clus = fat_dir_find(root(),name);
        printf_("lib:%s clus:%d\n",name,clus);
        if (!clus) return -1;
        binload(clus,currtask->buffer[i+1]);
    }
}
uint32_t init(uint32_t b,uint32_t c,uint32_t d)
{
    switch (b)
    {
    case 0:
        currtask->buffer = c;
        break;
    case 1:
        return loadlibs();
    }
    return 0;
}
