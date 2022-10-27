	.file	"main.c"
	.text
	.section	.text.main,"ax",@progbits
	.globl	main
	.type	main, @function
main:
	leal	4(%esp), %ecx
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	pushl	%ecx
	subl	$32, %esp
	call	debug_call
	subl	$12, %esp
	leal	-28(%ebp), %ebx
	pushl	%ebx
	call	init
	addl	$8, %esp
	pushl	$0
	pushl	%ebx
	call	clear
	addl	$16, %esp
	movl	$0, %eax
	leal	-8(%ebp), %esp
	popl	%ecx
	popl	%ebx
	popl	%ebp
	leal	-4(%ecx), %esp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 11.3.0"
