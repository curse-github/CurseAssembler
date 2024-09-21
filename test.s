	.file	"test.c"
	.text
	.globl	thing
	.data
	.align 4
	.type	thing, @object
	.size	thing, 4
thing:
	.long	15
	.globl	ptr
	.bss
	.align 8
	.type	ptr, @object
	.size	ptr, 8
ptr:
	.zero	8
	.text
	.globl	_start
	.type	_start, @function
_start:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	leaq	thing(%rip), %rax
	movq	%rax, ptr(%rip)
	movq	ptr(%rip), %rax
	movl	(%rax), %eax
	movl	%eax, %edi
	call	_exit@PLT
	.size	_start, .-_start
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
