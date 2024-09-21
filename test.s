	.file	"test.c"
	.text
	.globl	thing
	.data
	.align 4
thing:
	.long	15
	.globl	ptr
	.bss
	.align 8
ptr:
	.space 8
	.text
	.globl	_start
	.def	_start;	.scl	2;	.type	32;	.endef
_start:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	leaq	thing(%rip), %rax
	movq	%rax, ptr(%rip)
	movq	ptr(%rip), %rax
	movl	(%rax), %eax
	movl	%eax, %ecx
	call	_exit
	.ident	"GCC: (Rev3, Built by MSYS2 project) 13.2.0"
	.def	_exit;	.scl	2;	.type	32;	.endef
