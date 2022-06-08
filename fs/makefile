SRCFILES := $(shell find "." -type f -name "*.c")
ASMFILES := $(patsubst %.c,%.s,$(SRCFILES))
OBJFILES := $(patsubst %.c,%.o,$(SRCFILES))
#--print-map
all: s bin
	i386-elf-ld -Tlinker.ld $(OBJFILES) -o main.bin
	objdump -b binary --adjust-vma=0x8000000 -D main.bin -m i386

%.s: %.c
	i386-elf-gcc -ffunction-sections -fno-pie -Wall -O -static-libgcc -lgcc -fstrength-reduce -ffreestanding -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -fno-exceptions -fno-plt -fno-pic $< -c -S -o $@

%.o: %.s
	i386-elf-gcc -ffunction-sections -fno-pie -Wall -O -static-libgcc -lgcc -fstrength-reduce -ffreestanding -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -fno-exceptions -fno-plt -fno-pic $< -c -o $@
	objcopy --remove-section .comment $@

s: $(ASMFILES)
	./util/rgot

bin: $(OBJFILES)

clean:
	rm $(OBJFILES) $(ASMFILES)
