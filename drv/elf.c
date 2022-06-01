#include <system.h>
# define ELF_NIDENT	16
typedef uint16_t Elf32_Half;	// Unsigned half int
typedef uint32_t Elf32_Off;	// Unsigned offset
typedef uint32_t Elf32_Addr;	// Unsigned address
typedef uint32_t Elf32_Word;	// Unsigned int
typedef int32_t  Elf32_Sword;	// Signed int
typedef struct {
	uint8_t		e_ident[ELF_NIDENT];
	Elf32_Half	e_type;
	Elf32_Half	e_machine;
	Elf32_Word	e_version;
	Elf32_Addr	e_entry;
	Elf32_Off	e_phoff;
	Elf32_Off	e_shoff;
	Elf32_Word	e_flags;
	Elf32_Half	e_ehsize;
	Elf32_Half	e_phentsize;
	Elf32_Half	e_phnum;
	Elf32_Half	e_shentsize;
	Elf32_Half	e_shnum;
	Elf32_Half	e_shstrndx;
} Elf32_Ehdr;
# define ELFMAG0	0x7F // e_ident[EI_MAG0]
# define ELFMAG1	'E'  // e_ident[EI_MAG1]
# define ELFMAG2	'L'  // e_ident[EI_MAG2]
# define ELFMAG3	'F'  // e_ident[EI_MAG3]
bool elf_check_file(Elf32_Ehdr *hdr) {
	if(!hdr) return false;
	if(hdr->e_ident[0] != ELFMAG0) {
		puts("ELF Header EI_MAG0 incorrect.\n");
		return false;
	}
	if(hdr->e_ident[1] != ELFMAG1) {
		puts("ELF Header EI_MAG1 incorrect.\n");
		return false;
	}
	if(hdr->e_ident[2] != ELFMAG2) {
		puts("ELF Header EI_MAG2 incorrect.\n");
		return false;
	}
	if(hdr->e_ident[3] != ELFMAG3) {
		puts("ELF Header EI_MAG3 incorrect.\n");
		return false;
	}
	return true;
}
typedef struct {
	Elf32_Word		p_type;
	Elf32_Off		p_offset;
	Elf32_Addr		p_vaddr;
	Elf32_Addr		p_paddr;
	Elf32_Word		p_filesz;
	Elf32_Word		p_memsz;
	Elf32_Word		p_flags;
	Elf32_Word		p_align;
} Elf32_Phdr;
extern int sectors_per_cluster;
static inline Elf32_Phdr *elf_pheader(Elf32_Ehdr *hdr) {
	return (Elf32_Phdr *)((int)hdr + hdr->e_phoff);
}
int exec(int clus)
{
    uint8_t volatile * volatile target;
    read_sectors_ATA_PIO(target,0, cluster2sector(clus), sectors_per_cluster);
    /*for(int i = 0;i!=1;)
    {
        read_sectors_ATA_PIO(target,0, cluster2sector(clus), sectors_per_cluster);
        int clus = table_val(clus);
        printf_("clus:%x\n",clus);
        if(clus>0xfff6) i = 1;
    }*/
    /*int i = 0;
    while(i < 512*sectors_per_cluster)
    {
        printf_("%x",target[i]);
        i++;
    }*/
    //probe(target);
    printf_("%x\n",target);
    Elf32_Ehdr* hdr = (Elf32_Ehdr*)target;
    if(!elf_check_file(hdr)) return -1;
    //int (*entry)() = hdr + hdr->e_entry;
    //printf_("%x\n",hdr + hdr->);
    Elf32_Phdr *phdr = elf_pheader(hdr);
    unsigned int i;
    // Iterate over section headers
    for(i = 0; i < hdr->e_phnum; i++) {
	Elf32_Phdr *prog = &phdr[i];
        printf_("phead vadr:%x\n",prog->p_vaddr);
        printf_("phead offset:%x\n",prog->p_offset);
        printf_("phead filesz:%x\n",prog->p_filesz);
        printf_("phead src:%x\n",(int)target + (int)prog->p_offset);
        i++;
        /*int i = prog->p_offset;
        while(i < prog->p_offset + prog->p_filesz)
        {
            printf_("%x",target[i]);
            i++;
        }*/
        memcpy(prog->p_vaddr,target + prog->p_offset,prog->p_filesz);
    }
    printf_("%x\n",table_val(clus));
    printf_("%x\n",hdr->e_entry);
    outportw(0x8A00,0x8A00); outportw(0x8A00,0x08AE0);
    int (*entry)() = hdr->e_entry;
    entry();
}
