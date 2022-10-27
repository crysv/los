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
#define PCI_GET_BAR0( bus, dev, fun ) pci_read_dword( bus, dev, fun, 0x10 )
#define PCI_GET_BAR1( bus, dev, fun ) pci_read_dword( bus, dev, fun, 0x14 )
#define PCI_GET_BAR2( bus, dev, fun ) pci_read_dword( bus, dev, fun, 0x18 )
#define PCI_GET_BAR3( bus, dev, fun ) pci_read_dword( bus, dev, fun, 0x1c )
#define PCI_GET_BAR4( bus, dev, fun ) pci_read_dword( bus, dev, fun, 0x20 )
#define PCI_GET_BAR5( bus, dev, fun ) pci_read_dword( bus, dev, fun, 0x24 )
void checkfunc(uint8_t bus,uint8_t dev,uint8_t fun)
{
    uint8_t subclass = PCI_GET_SUBCLASS( bus, dev, fun );
    uint8_t class = PCI_GET_CLASS( bus, dev, fun );
    uint8_t progif = PCI_GET_PROGIF( bus, dev, fun );
    //printf_("bus:%02x dev:%02x func:%02x class:%02x subclass:%02x progif:%02x\n",
    //         bus,dev,fun,class,subclass,progif);
    uint32_t bar[6] = {PCI_GET_BAR0( bus, dev, fun ),
                       PCI_GET_BAR1( bus, dev, fun ),
                       PCI_GET_BAR2( bus, dev, fun ),
                       PCI_GET_BAR3( bus, dev, fun ),
                       PCI_GET_BAR4( bus, dev, fun ),
                       PCI_GET_BAR5( bus, dev, fun )};
    //printf_("bar0:%x bar1:%x bar2:%x bar3:%x bar4:%x bar5:%x\n",
    //         bar[0],bar[1],bar[2],bar[3],bar[4],bar[5]);
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
                 for (func = 1; func < 8; func++) {
                     if(PCI_GET_VENDOR( bus, device, func)==0xffff) continue;
                     checkfunc(bus,device,func);
                 }
             }
         }
     }
 }
