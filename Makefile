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
#--print-map
	ld -melf_i386 -T link.ld -o kernel.bin $(OBJFILES)
	nm  kernel.bin | grep " T " | sed -r 's/(\s+)?\S+//2' > kernel.sym

%.o: %.c
	i386-elf-gcc -Wall -Wno-implicit-function-declaration -O -fstrength-reduce -ffreestanding -fomit-frame-pointer -finline-functions -I./include -c -o $@ $< -lgcc -lc

%.o: %.asm
	nasm -f elf32 -o $@ $<

clean:
	rm  drv/*.o libc/*.o sys/*.o *.o kernel.bin

unlock:
	rm *.lock fs/*.lock
