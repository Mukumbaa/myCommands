	.file	"main.c"
	.intel_syntax noprefix
	.text
	.section .rdata,"dr"
	.align 8
.LC0:
	.ascii "-fno-asynchronous-unwind-tables\0"
.LC1:
	.ascii "-S\0"
.LC2:
	.ascii "gcc\0"
.LC3:
	.ascii "-masm=intel\0"
.LC4:
	.ascii "output.asm\0"
.LC5:
	.ascii "-o\0"
	.text
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 96
	mov	DWORD PTR 16[rbp], ecx
	mov	QWORD PTR 24[rbp], rdx
	call	__main
	mov	rax, QWORD PTR 24[rbp]
	mov	rax, QWORD PTR 8[rax]
	mov	QWORD PTR -8[rbp], rax
	mov	QWORD PTR 64[rsp], 0
	lea	rax, .LC3[rip]
	mov	QWORD PTR 56[rsp], rax
	lea	rax, .LC4[rip]
	mov	QWORD PTR 48[rsp], rax
	lea	rax, .LC5[rip]
	mov	QWORD PTR 40[rsp], rax
	mov	rax, QWORD PTR -8[rbp]
	mov	QWORD PTR 32[rsp], rax
	lea	r9, .LC0[rip]
	lea	r8, .LC1[rip]
	lea	rax, .LC2[rip]
	mov	rdx, rax
	lea	rax, .LC2[rip]
	mov	rcx, rax
	call	execlp
	mov	eax, 0
	leave
	ret
	.def	__main;	.scl	2;	.type	32;	.endef
	.ident	"GCC: (x86_64-posix-seh-rev2, Built by MinGW-Builds project) 14.2.0"
	.def	execlp;	.scl	2;	.type	32;	.endef
