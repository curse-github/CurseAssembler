	.file	"test.c"
	.text
	.globl	_start
	.def	_start;	.scl	2;	.type	32;	.endef
_start:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	movl	$15, %ecx
	call	_exit
	.ident	"GCC: (Rev3, Built by MSYS2 project) 13.2.0"
	.def	_exit;	.scl	2;	.type	32;	.endef
