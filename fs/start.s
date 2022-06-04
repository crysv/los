	.file	"start.c"
	.text
	.section	.text.outportw,"ax",@progbits
	.globl	outportw
	.type	outportw, @function
outportw:
	movl	4(%esp), %edx
	movl	8(%esp), %eax
/APP
/  11 "start.c" 1
	outw %ax, %dx
/  0 "" 2
/NO_APP
	ret
	.size	outportw, .-outportw
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"puts"
.LC1:
	.string	"kmalloc"
.LC2:
	.string	"kfree"
	.section	.text.start,"ax",@progbits
	.globl	start
	.type	start, @function
start:
	pushl	%ebx
	subl	$20, %esp
	movl	%ecx, %ebx
	pushl	$.LC0
	call	*%ecx
	movl	%eax, puts
	pushl	$.LC1
	call	*%ebx
	movl	%eax, kmalloc
	pushl	$.LC2
	call	*%ebx
	movl	%eax, kfree
	movl	$-30208, %edx
	movl	%edx, %eax
/APP
/  11 "start.c" 1
	outw %ax, %dx
/  0 "" 2
/NO_APP
	movl	$-29984, %eax
/APP
/  11 "start.c" 1
	outw %ax, %dx
/  0 "" 2
/  20 "start.c" 1
	call main
/  0 "" 2
/NO_APP
	addl	$20, %esp
	popl	%ebx
	ret
	.size	start, .-start
	.section	.text.some_call,"ax",@progbits
	.globl	some_call
	.type	some_call, @function
some_call:
	ret
	.size	some_call, .-some_call
	.globl	kfree
	.section	.bss
	.align 4
	.type	kfree, @object
	.size	kfree, 4
kfree:
	.zero	4
	.globl	kmalloc
	.align 4
	.type	kmalloc, @object
	.size	kmalloc, 4
kmalloc:
	.zero	4
	.globl	puts
	.align 4
	.type	puts, @object
	.size	puts, 4
puts:
	.zero	4
	.ident	"GCC: (GNU) 11.3.0"
