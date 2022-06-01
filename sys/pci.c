#include <system.h>
uint8_t pci_read_byte( int bus, int dev, int fun, int off )
{
    outportl( 0xcf8, (1 << 31) | (bus << 16) | (dev << 11) | (fun << 8) | (off & 0xfc) );
    return inportb( 0xcfc + (off & 0x03) );
}

uint16_t pci_read_word( int bus, int dev, int fun, int off )
{
    if( (off & 0x03) == 0x3 )
    {
        return 0; // todo: split into multiple reads to cross a 32-bit boundary
    }
    outportl( 0xcf8, (1 << 31) | (bus << 16) | (dev << 11) | (fun << 8) | (off & 0xfc) );
    return inportw( 0xcfc + (off & 0x03) );
}
uint32_t pci_read_dword( int bus, int dev, int fun, int off )
{
    if( off & 0x03 )
    {
        return 0; // todo: split into multiple reads to cross a 32-bit boundary
    }
    outportl( 0xcf8, (1 << 31) | (bus << 16) | (dev << 11) | (fun << 8) | (off & 0xfc) );
    return inportl( 0xcfc );
}
#define PCI_GET_VENDOR( bus, dev, fun ) pci_read_word( bus, dev, fun, 0x0 )
#define PCI_GET_PROGIF( bus, dev, fun ) pci_read_byte( bus, dev, fun, 0x9 )
#define PCI_GET_SUBCLASS( bus, dev, fun ) pci_read_byte( bus, dev, fun, 0xa )
#define PCI_GET_CLASS( bus, dev, fun ) pci_read_byte( bus, dev, fun, 0xb )
void checkfunc(uint8_t bus,uint8_t dev,uint8_t fun)
{
    uint8_t subclass = PCI_GET_SUBCLASS( bus, dev, fun );
    uint8_t class = PCI_GET_CLASS( bus, dev, fun );
    uint8_t progif = PCI_GET_PROGIF( bus, dev, fun );
    printf_("bus:%02x dev:%02x func:%02x class:%02x subclass:%02x progif:%02x\n",
             bus,dev,fun,class,subclass,progif);
}
void checkAllBuses(void) {
     uint16_t bus;
     uint8_t device;
     uint8_t func;
 
     for (bus = 0; bus < 256; bus++) {
         for (device = 0; device < 32; device++) {
             if(PCI_GET_VENDOR( bus, device, 0)==0xffff) continue;
             checkfunc(bus,device,0);
             if( pci_read_byte( bus, device, 0, 0xe ) & 0x80 )
             {
                 for (func = 0; func < 8; func++) {
                     if(PCI_GET_VENDOR( bus, device, func)==0xffff) continue;
                     checkfunc(bus,device,func);
                 }
             }
         }
     }
 }
