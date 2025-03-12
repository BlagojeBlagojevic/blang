segment .text

PRINT_INT:
	sub     rsp, 40
	xor     ecx, ecx
	mov     r8d, 10
	mov     rsi, rsp
	lea     r10, [rsp+31]
.L2:
	mov     rax, rdi
	xor     edx, edx
	mov     r9, r10
	div     r8
	sub     r9, rcx
	inc     rcx
	add     edx, 48
	mov     BYTE [r9], dl
	mov     rdx, rdi
	mov     rdi, rax
	cmp     rdx, 9
	ja      .L2
	sub     rsi, rcx
	mov     rdx, rcx
	mov     edi, 1
	add     rsi, 32
	mov rax, 1
	syscall
	add     rsp, 40
	ret
global _start
_start:

	;;push 0
	push 0

	;;push 10
	push 10

	;;push 20
	push 20

	;;add
	pop rax
	pop rbx
	add rax, rbx
	push rax

	;;print
	pop rdi
	call PRINT_INT

	;;push 10
	push 10

	;;push 20
	push 20

	;;push 30
	push 30

	;;sub
	pop rax
	pop rbx
	sub rax, rbx
	push rax

	;;print
	pop rdi
	call PRINT_INT
	mov rax, 60
	mov rdi, 0
	syscall
