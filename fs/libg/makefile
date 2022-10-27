LIBFILES := $(shell find "." -type f -name "*.c")
OBJFILES := $(patsubst %.c,%.o,$(LIBFILES))
all: $(OBJFILES)
	i386-elf-ld -n --print-map -Tlinker.ld $(OBJFILES) -o libg.bin
	objdump -b binary --adjust-vma=0x0 -D libg.bin -m i386
	nm  libg.o | grep " T " | sed -r 's/(\s+)?\S+//2' > libg.sym

%.o: %.c
	i386-elf-gcc -fpie -Wall -O -static-libgcc -lgcc -fstrength-reduce -ffreestanding -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -fno-exceptions -fno-plt -fpic $< -c -o $@
	objcopy --remove-section .comment $@

clean:
	rm $(OBJFILES) libg.bin
