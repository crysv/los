#include <system.h>
typedef struct AcpiHeader
{
    uint32_t signature;
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    uint8_t oem[6];
    uint8_t oemTableId[8];
    uint32_t oemRevision;
    uint32_t creatorId;
    uint32_t creatorRevision;
} __attribute__((packed)) AcpiHeader;
uint8_t lapic_ids[256]={0}; // CPU core Local APIC IDs
uint8_t numcore=0;          // number of cores detected
uint64_t lapic_ptr=0;       // pointer to the Local APIC MMIO registers
uint64_t ioapic_ptr=0;      // pointer to the IO APIC MMIO registers
void AcpiParseApic(uint8_t* madt)
{
  lapic_ptr = (uint64_t)(*((uint32_t*)(madt+0x24)));
  uint64_t lapic_preserve = lapic_ptr;
  printf_("lapic should be %x\n",lapic_ptr);
  uint8_t* ptr2 = madt + *((uint32_t*)(madt + 4));
  // iterate on variable length records
  for(madt += 44; madt < ptr2; madt += madt[1]) {
    switch(madt[0]) {
      case 0: if(madt[4] & 1) lapic_ids[numcore++] = madt[3]; break; // found Processor Local APIC
      case 1: ioapic_ptr = (uint64_t)*((uint32_t*)(madt+4)); break;  // found IOAPIC
      case 5: lapic_ptr = *((uint64_t*)(madt+4)); puts("change");break;             // found 64 bit LAPIC
    }
  } 
  printf_("Found %d cores, IOAPIC %lx, "  , numcore, ioapic_ptr);
  printf_("LAPIC %lx, Processor IDs:", lapic_ptr);
  for(int i = 0; i < numcore; i++)
    printf_(" %d", lapic_ids[i]);
  printf_("\n");
}
void AcpiParseDT(AcpiHeader *header)
{
    uint32_t signature = header->signature;

    char sigStr[5];
    memcpy(sigStr, &signature, 4);
    sigStr[4] = 0;
    printf_("%s 0x%x\n", sigStr, signature);

    if (signature == 0x50434146)
    {
        //AcpiParseFacp((AcpiFadt *)header);
    }
    else if (signature == 0x43495041)
    {
        //AcpiParseApic((uint8_t*)header);
    }
}
void AcpiParseRsdt(AcpiHeader *rsdt)
{
    uint32_t *p = (uint32_t *)(rsdt + 1);
    uint32_t *end = (uint32_t *)((uint8_t*)rsdt + rsdt->length);

    while (p < end)
    {
        uint32_t address = *p++;
        AcpiParseDT((AcpiHeader *)(uintptr_t)address);
    }
}
bool parse_rsdp(uint8_t *p)
{
    uint8_t sum = 0;
    for (unsigned int i = 0; i < 20; ++i)
    {
        sum += p[i];
    }
    if (sum)
    {
        puts("rsdp not valid");
        return false;
    }
    // Print OEM
    char oem[7];
    memcpy(oem, p + 9, 6);
    oem[6] = '\0';
    printf_("OEM = %s\n", oem);

    // Check version
    uint8_t revision = p[15];
    if (revision == 0)
    {
        puts("Version 1\n");

        uint32_t rsdtAddr = *(uint32_t *)(p + 16);
        AcpiParseRsdt((AcpiHeader *)(uintptr_t)rsdtAddr);
    }
    else if (revision == 2)
    {
        puts("Version 2\n");

        uint32_t rsdtAddr = *(uint32_t *)(p + 16);
        uint64_t xsdtAddr = *(uint64_t *)(p + 24);

        if (xsdtAddr)
        {
            //AcpiParseXsdt((AcpiHeader *)(uintptr_t)xsdtAddr);
        }
        else
        {
            AcpiParseRsdt((AcpiHeader *)(uintptr_t)rsdtAddr);
        }
    }
    else
    {
        printf_("Unsupported ACPI version %d\n", revision);
    }
    
    return true;
}
void acpi_init()
{
    // TODO - Search Extended BIOS Area
    
    // Search main BIOS area below 1MB
    uint8_t *p = (uint8_t *)0x000e0000;
    uint8_t *end = (uint8_t *)0x000fffff;

    while (p < end)
    {
        uint64_t signature = *(uint64_t *)p;

        if (signature == 0x2052545020445352) // 'RSD PTR '
        {
            printf_("found Rsdp: 0x%02x\n",p);
            if (parse_rsdp(p))
            {
                break;
            }
        }
        else if (signature == 0x5F504D5F) // '_MP_'
        {
            printf_("found mp: 0x%02x\n",p);
        }
        p += 16;
    }
}
