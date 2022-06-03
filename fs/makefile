all:
	i386-elf-gcc -fno-pie -Wall -O -static-libgcc -lgcc -fstrength-reduce -ffreestanding -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -fno-exceptions -fno-plt -fno-pic -ffunction-sections start.c -c -o start.o
	#i386-elf-gcc -ffunction-sections -fno-pie -Wall -O -static-libgcc -lgcc -fstrength-reduce -ffreestanding -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -fno-exceptions -fno-plt -fno-pic main.c -c -o main.o
	#objcopy --remove-section .comment main.o
	objcopy --remove-section .comment start.o
	i386-elf-ld -Tlinker.ld start.o -o main.bin
	objdump -b binary --adjust-vma=0x200000 -D main.bin -m i386
