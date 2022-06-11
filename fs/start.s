	.file	"start.c"
	.text
	.section	.text.outportw,"ax",@progbits
	.globl	outportw
	.type	outportw, @function
outportw:
	movl	4(%esp), %edx
	movl	8(%esp), %eax
/APP
/  8 "start.c" 1
	outw %ax, %dx
/  0 "" 2
/NO_APP
	ret
	.size	outportw, .-outportw
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"LIB     BIN"
	.section	.text.start,"ax",@progbits
	.globl	start
	.type	start, @function
start:
	pushl	%ebx
	subl	$20, %esp
	movl	$0, %eax
	movl	$_buf, %ecx
	movl	%eax, %ebx
	movl	%eax, %edx
/APP
/  15 "start.c" 1
	int $0x7f
/  0 "" 2
/NO_APP
	movl	$.LC0, _buf
	movl	$libbase, _buf+4
	movl	$0, _buf+8
	movl	$0, _buf+12
	movl	$1, %ebx
	movl	%eax, %ecx
/APP
/  20 "start.c" 1
	int $0x7f
/  0 "" 2
/NO_APP
	movl	$-30208, %edx
	movl	%edx, %eax
/APP
/  8 "start.c" 1
	outw %ax, %dx
/  0 "" 2
/NO_APP
	movl	$-29984, %eax
/APP
/  8 "start.c" 1
	outw %ax, %dx
/  0 "" 2
/  23 "start.c" 1
	call main
/  0 "" 2
/NO_APP
	pushl	%eax
	call	exit
	addl	$24, %esp
	popl	%ebx
	ret
	.size	start, .-start
	.section	.text.debug_call,"ax",@progbits
	.globl	debug_call
	.type	debug_call, @function
debug_call:
	movl	$-30208, %edx
	movl	%edx, %eax
/APP
/  8 "start.c" 1
	outw %ax, %dx
/  0 "" 2
/NO_APP
	movl	$-29984, %eax
/APP
/  8 "start.c" 1
	outw %ax, %dx
/  0 "" 2
/NO_APP
	ret
	.size	debug_call, .-debug_call
	.globl	_buf
	.section	.bss
	.align 32
	.type	_buf, @object
	.size	_buf, 128
_buf:
	.zero	128
	.ident	"GCC: (GNU) 11.3.0"
