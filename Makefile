PROJDIRS := sys/ libc/ drv/
SRCFILES := $(shell find -not -path "./fs/*" -type f -name "*.c")
HDRFILES := $(shell find include/ -type f -name "*.h")
OBJFILES := start.o $(patsubst %.c,%.o,$(SRCFILES)) font/font.o
ALLFILES := $(SRCFILES) $(HDRFILES) bochsrc.txt link.ld grub.cfg makefile

all: img

img: kernel.bin
	mkdir -p isodir/boot/grub
	cp kernel.bin isodir/boot/kernel.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o iso.iso isodir
	# qemu-img resize iso.iso +10M

kernel.bin: $(OBJFILES)
#start.o sys/arith64.o sys/acpi.o libc/libc.o font/font.o drv/scrn.o libc/printf.o main.o sys/gdt.o sys/idt.o sys/isrs.o sys/pci.o sys/irq.o drv/timer.o drv/kb.o drv/atapio.o drv/lfs.o
	ld -melf_i386 -T link.ld -o kernel.bin $(OBJFILES)
#start.o sys/arith64.o sys/acpi.o libc/libc.o font/font.o drv/scrn.o libc/printf.o main.o sys/gdt.o sys/idt.o sys/isrs.o sys/pci.o sys/irq.o drv/timer.o drv/kb.o drv/atapio.o drv/lfs.o

%.o: %.c
	i386-elf-gcc -Wall -Wno-implicit-function-declaration -O -static-libgcc -lgcc -fstrength-reduce -ffreestanding -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o $@ $<

%.o: %.asm
	nasm -f elf32 -o $@ $<

clean:
	rm  drv/*.o libc/*.o sys/*.o *.o kernel.bin

unlock:
	rm *.lock fs/*.lock
