	.file	"lib.c"
	.text
	.section	.text.malloc,"ax",@progbits
	.globl	malloc
	.type	malloc, @function
malloc:
	pushl	%ebx
	movl	8(%esp), %ecx
	movl	$1, %eax
	movl	$0, %ebx
/APP
/  9 "lib.c" 1
	int $0x7f
/  0 "" 2
/NO_APP
	popl	%ebx
	ret
	.size	malloc, .-malloc
	.section	.text.free,"ax",@progbits
	.globl	free
	.type	free, @function
free:
	pushl	%ebx
	movl	$2, %eax
	movl	$0, %ecx
	movl	8(%esp), %ebx
/APP
/  10 "lib.c" 1
	int $0x7f
/  0 "" 2
/NO_APP
	movl	%eax, _dummy
	popl	%ebx
	ret
	.size	free, .-free
	.section	.text.exit,"ax",@progbits
	.globl	exit
	.type	exit, @function
exit:
	pushl	%ebx
	movl	$3, %eax
	movl	$0, %ecx
	movl	8(%esp), %ebx
/APP
/  11 "lib.c" 1
	int $0x7f
/  0 "" 2
/NO_APP
	movl	%eax, _dummy
	popl	%ebx
	ret
	.size	exit, .-exit
	.globl	_dummy
	.section	.bss
	.align 4
	.type	_dummy, @object
	.size	_dummy, 4
_dummy:
	.zero	4
	.ident	"GCC: (GNU) 11.3.0"
