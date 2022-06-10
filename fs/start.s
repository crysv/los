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
	.section	.text.start,"ax",@progbits
	.globl	start
	.type	start, @function
start:
	pushl	%ebx
	subl	$20, %esp
	movl	$0, %eax
	movl	%eax, %ebx
	movl	%eax, %ecx
/APP
/  14 "start.c" 1
	int $0x7f
/  0 "" 2
/  17 "start.c" 1
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
	.ident	"GCC: (GNU) 11.3.0"
