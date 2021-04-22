	.file	"volatile.cc"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB12:
	.cfi_startproc
	cmpb	$0, quit(%rip)
	jne	.L8
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	.p2align 4,,10
	.p2align 3
.L6:
	movl	$1, %edi
	call	sleep
	cmpb	$0, quit(%rip)
	je	.L6
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
.L8:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE12:
	.size	main, .-main
	.globl	quit
	.bss
	.type	quit, @object
	.size	quit, 1
quit:
	.zero	1
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-16)"
	.section	.note.GNU-stack,"",@progbits
