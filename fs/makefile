all:
	i386-elf-gcc -fno-pie -Wall -O -static-libgcc -lgcc -fstrength-reduce -ffreestanding -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -fno-exceptions -fno-plt -fpic main.c -r -c -o main.o
	objcopy --remove-section .comment main.o
	i386-elf-ld -Tlinker.ld main.o -o main.bin
	objdump -b binary --adjust-vma=0x0 -D main.bin -m i386

