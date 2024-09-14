	.file	"test.c"
	.text
	.globl	code
	.data
	.align 4
code:
	.long	15
	.globl	codeptr
	.align 8
codeptr:
	.quad	code
	.text
	.globl	_start
	.def	_start;	.scl	2;	.type	32;	.endef
_start:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	movq	codeptr(%rip), %rax
	movl	(%rax), %eax
	addl	$2, %eax
	movl	%eax, %ecx
	call	_exit
	.ident	"GCC: (Rev3, Built by MSYS2 project) 13.2.0"
	.def	_exit;	.scl	2;	.type	32;	.endef
