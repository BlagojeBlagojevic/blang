section .bss
	new_stack resq 1000000  ; Reserve 1MB of qword for the new stack
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

PRINT_CHAR:
	sub     rsp, 24
	mov     edx, 1
	mov     BYTE  [rsp+15], dil
	lea     rsi, [rsp+15]
	mov     edi, 1
	mov rax, 1
	syscall
	add     rsp, 24
	ret


COMPARE:
	test    rdx, rdx
	jne     .L2
	cmp     rsi, rdi
	jmp     .L6
.L2:
	dec     rdx
	jne     .L4
	cmp     rdi, rsi
.L6:
	setb    al
	jmp     .L5
.L4:
	cmp     rdi, rsi
	sete    al
.L5:
	movzx   eax, al   ; Zero-extend AL into EAX
	ret               ; Return to the caller
global _start
_start:
; Switch to new stack (aligned)
	mov rsp, new_stack + 1000000

	;;push 0
.LABEL0:
	push 0

	;;push 35
.LABEL1:
	push 35

	;;NOT IMPLEMENTED NOP
.LABEL2:

	;;mem 200023
.LABEL3:
	pop rax
	mov qword [new_stack + (200023 * 8)], rax  ; Store at explicit address
	push rax

	;;push 32
.LABEL4:
	push 32

	;;NOT IMPLEMENTED NOP
.LABEL5:

	;;mem 200022
.LABEL6:
	pop rax
	mov qword [new_stack + (200022 * 8)], rax  ; Store at explicit address
	push rax

	;;push 10
.LABEL7:
	push 10

	;;NOT IMPLEMENTED NOP
.LABEL8:

	;;mem 200021
.LABEL9:
	pop rax
	mov qword [new_stack + (200021 * 8)], rax  ; Store at explicit address
	push rax

	;;push 0
.LABEL10:
	push 0

	;;NOT IMPLEMENTED NOP
.LABEL11:

	;;mem 200020
.LABEL12:
	pop rax
	mov qword [new_stack + (200020 * 8)], rax  ; Store at explicit address
	push rax

	;;push 1
.LABEL13:
	push 1

	;;NOT IMPLEMENTED NOP
.LABEL14:

	;;mem 200019
.LABEL15:
	pop rax
	mov qword [new_stack + (200019 * 8)], rax  ; Store at explicit address
	push rax

	;;push 1
.LABEL16:
	push 1

	;;NOT IMPLEMENTED NOP
.LABEL17:

	;;mem 200018
.LABEL18:
	pop rax
	mov qword [new_stack + (200018 * 8)], rax  ; Store at explicit address
	push rax

	;;push 0
.LABEL19:
	push 0

	;;NOT IMPLEMENTED NOP
.LABEL20:

	;;mem 200017
.LABEL21:
	pop rax
	mov qword [new_stack + (200017 * 8)], rax  ; Store at explicit address
	push rax

	;;push 1
.LABEL22:
	push 1

	;;NOT IMPLEMENTED NOP
.LABEL23:

	;;mem 200016
.LABEL24:
	pop rax
	mov qword [new_stack + (200016 * 8)], rax  ; Store at explicit address
	push rax

	;;push 1
.LABEL25:
	push 1

	;;NOT IMPLEMENTED NOP
.LABEL26:

	;;mem 200015
.LABEL27:
	pop rax
	mov qword [new_stack + (200015 * 8)], rax  ; Store at explicit address
	push rax

	;;push 1
.LABEL28:
	push 1

	;;NOT IMPLEMENTED NOP
.LABEL29:

	;;mem 200014
.LABEL30:
	pop rax
	mov qword [new_stack + (200014 * 8)], rax  ; Store at explicit address
	push rax

	;;push 0
.LABEL31:
	push 0

	;;NOT IMPLEMENTED NOP
.LABEL32:

	;;mem 200013
.LABEL33:
	pop rax
	mov qword [new_stack + (200013 * 8)], rax  ; Store at explicit address
	push rax

	;;NOT IMPLEMENTED NOP
.LABEL34:

	;;pop 0
.LABEL35:
	pop rax

	;;push 100
.LABEL36:
	push 100

	;;NOT IMPLEMENTED NOP
.LABEL37:

	;;pop 0
.LABEL38:
	pop rax

	;;push 200
.LABEL39:
	push 200

	;;push 1
.LABEL40:
	push 1

	;;NOT IMPLEMENTED NOP
.LABEL41:

	;;mem 199912
.LABEL42:
	pop rax
	mov qword [new_stack + (199912 * 8)], rax  ; Store at explicit address
	push rax

	;;push 0
.LABEL43:
	push 0

	;;NOT IMPLEMENTED NOP
.LABEL44:

	;;mem 199710
.LABEL45:
	pop rax
	mov qword [new_stack + (199710 * 8)], rax  ; Store at explicit address
	push rax

	;;push 0
.LABEL46:
	push 0

	;;NOT IMPLEMENTED NOP
.LABEL47:

	;;mem 199709
.LABEL48:
	pop rax
	mov qword [new_stack + (199709 * 8)], rax  ; Store at explicit address
	push rax

	;;push 0
.LABEL49:
	push 0

	;;NOT IMPLEMENTED NOP
.LABEL50:

	;;mem 199708
.LABEL51:
	pop rax
	mov qword [new_stack + (199708 * 8)], rax  ; Store at explicit address
	push rax

	;;NOT IMPLEMENTED NOP
.LABEL52:

	;;NOT IMPLEMENTED NOP
.LABEL53:

	;;NOT IMPLEMENTED NOP
.LABEL54:

	;;push 199912
.LABEL55:
	push 199912

	;;copy 199709
.LABEL56:
	mov rax, qword [new_stack + (199709 * 8)]   ; Load from explicit address
	push rax

	;;add
.LABEL57:
	pop rax
	pop rbx
	add rax, rbx
	push rax

	;;copystack 0
.LABEL58:pop rax
	shl rax, 3
	mov rbx, [new_stack + rax]
	push rbx

	;;NOT IMPLEMENTED NOP
.LABEL59:

	;;mem 199708
.LABEL60:
	pop rax
	mov qword [new_stack + (199708 * 8)], rax  ; Store at explicit address
	push rax

	;;NOT IMPLEMENTED NOP
.LABEL61:

	;;push 199912
.LABEL62:
	push 199912

	;;copy 199709
.LABEL63:
	mov rax, qword [new_stack + (199709 * 8)]   ; Load from explicit address
	push rax

	;;add
.LABEL64:
	pop rax
	pop rbx
	add rax, rbx
	push rax

	;;push 1
.LABEL65:
	push 1

	;;add
.LABEL66:
	pop rax
	pop rbx
	add rax, rbx
	push rax

	;;copystack 0
.LABEL67:pop rax
	shl rax, 3
	mov rbx, [new_stack + rax]
	push rbx

	;;push 2
.LABEL68:
	push 2

	;;mul
.LABEL69:
	pop rax
	pop rdi
	imul rax, rdi
	push rax

	;;copy 199708
.LABEL70:
	mov rax, qword [new_stack + (199708 * 8)]   ; Load from explicit address
	push rax

	;;add
.LABEL71:
	pop rax
	pop rbx
	add rax, rbx
	push rax

	;;NOT IMPLEMENTED NOP
.LABEL72:

	;;mem 199708
.LABEL73:
	pop rax
	mov qword [new_stack + (199708 * 8)], rax  ; Store at explicit address
	push rax

	;;NOT IMPLEMENTED NOP
.LABEL74:

	;;push 199912
.LABEL75:
	push 199912

	;;copy 199709
.LABEL76:
	mov rax, qword [new_stack + (199709 * 8)]   ; Load from explicit address
	push rax

	;;add
.LABEL77:
	pop rax
	pop rbx
	add rax, rbx
	push rax

	;;push 2
.LABEL78:
	push 2

	;;add
.LABEL79:
	pop rax
	pop rbx
	add rax, rbx
	push rax

	;;copystack 0
.LABEL80:pop rax
	shl rax, 3
	mov rbx, [new_stack + rax]
	push rbx

	;;push 4
.LABEL81:
	push 4

	;;mul
.LABEL82:
	pop rax
	pop rdi
	imul rax, rdi
	push rax

	;;copy 199708
.LABEL83:
	mov rax, qword [new_stack + (199708 * 8)]   ; Load from explicit address
	push rax

	;;add
.LABEL84:
	pop rax
	pop rbx
	add rax, rbx
	push rax

	;;NOT IMPLEMENTED NOP
.LABEL85:

	;;mem 199708
.LABEL86:
	pop rax
	mov qword [new_stack + (199708 * 8)], rax  ; Store at explicit address
	push rax

	;;NOT IMPLEMENTED NOP
.LABEL87:

	;;push 200013
.LABEL88:
	push 200013

	;;copy 199708
.LABEL89:
	mov rax, qword [new_stack + (199708 * 8)]   ; Load from explicit address
	push rax

	;;add
.LABEL90:
	pop rax
	pop rbx
	add rax, rbx
	push rax

	;;copystack 0
.LABEL91:pop rax
	shl rax, 3
	mov rbx, [new_stack + rax]
	push rbx

	;;NOT IMPLEMENTED NOP
.LABEL92:

	;;mem 199708
.LABEL93:
	pop rax
	mov qword [new_stack + (199708 * 8)], rax  ; Store at explicit address
	push rax

	;;push 1
.LABEL94:
	push 1

	;;if 2
.LABEL95:
	pop rdi
	pop rsi
	mov rdx, 2
	call COMPARE
	push rax

	;;dup
.LABEL96:
	pop rax
	push rax
	push rax

	;;jmpf
.LABEL97:
	pop rax
	test rax, rax   ; Check if false (0)
	jz .LABEL100

	;;copy 200023
.LABEL98:
	mov rax, qword [new_stack + (200023 * 8)]   ; Load from explicit address
	push rax

	;;printChar
.LABEL99:
	mov rax, 1
	mov rdi, 1
	mov rsi, rsp
	mov rdx, 1
	syscall

	;;NOT IMPLEMENTED NOP
.LABEL100:

	;;NOT IMPLEMENTED SETSP
.LABEL101:

	;;push 0
.LABEL102:
	push 0

	;;if 2
.LABEL103:
	pop rdi
	pop rsi
	mov rdx, 2
	call COMPARE
	push rax

	;;jmpf
.LABEL104:
	pop rax
	test rax, rax   ; Check if false (0)
	jz .LABEL107

	;;copy 200022
.LABEL105:
	mov rax, qword [new_stack + (200022 * 8)]   ; Load from explicit address
	push rax

	;;printChar
.LABEL106:
	mov rax, 1
	mov rdi, 1
	mov rsi, rsp
	mov rdx, 1
	syscall

	;;NOT IMPLEMENTED NOP
.LABEL107:

	;;copy 199708
.LABEL108:
	mov rax, qword [new_stack + (199708 * 8)]   ; Load from explicit address
	push rax

	;;NOT IMPLEMENTED NOP
.LABEL109:

	;;push 199711
.LABEL110:
	push 199711

	;;copy 199709
.LABEL111:
	mov rax, qword [new_stack + (199709 * 8)]   ; Load from explicit address
	push rax

	;;add
.LABEL112:
	pop rax
	pop rbx
	add rax, rbx
	push rax

	;;push 1
.LABEL113:
	push 1

	;;add
.LABEL114:
	pop rax
	pop rbx
	add rax, rbx
	push rax

	;;memstack 0
.LABEL115:pop rax
	shl rax, 3
	pop rbx
	mov [new_stack + rax], rbx
	push rbx

	;;push 0
.LABEL116:
	push 0

	;;NOT IMPLEMENTED NOP
.LABEL117:

	;;mem 199708
.LABEL118:
	pop rax
	mov qword [new_stack + (199708 * 8)], rax  ; Store at explicit address
	push rax

	;;push 1
.LABEL119:
	push 1

	;;copy 199709
.LABEL120:
	mov rax, qword [new_stack + (199709 * 8)]   ; Load from explicit address
	push rax

	;;add
.LABEL121:
	pop rax
	pop rbx
	add rax, rbx
	push rax

	;;NOT IMPLEMENTED NOP
.LABEL122:

	;;mem 199709
.LABEL123:
	pop rax
	mov qword [new_stack + (199709 * 8)], rax  ; Store at explicit address
	push rax

	;;push 100
.LABEL124:
	push 100

	;;copy 199709
.LABEL125:
	mov rax, qword [new_stack + (199709 * 8)]   ; Load from explicit address
	push rax

	;;if 0
.LABEL126:
	pop rdi
	pop rsi
	mov rdx, 0
	call COMPARE
	push rax

	;;jmpf
.LABEL127:
	pop rax
	test rax, rax   ; Check if false (0)
	jz .LABEL132

	;;push 0
.LABEL128:
	push 0

	;;NOT IMPLEMENTED NOP
.LABEL129:

	;;mem 199709
.LABEL130:
	pop rax
	mov qword [new_stack + (199709 * 8)], rax  ; Store at explicit address
	push rax

	;;jmp
.LABEL131:
	jmp .LABEL134

	;;NOT IMPLEMENTED NOP
.LABEL132:

	;;jmp
.LABEL133:
	jmp .LABEL53

	;;copy 200021
.LABEL134:
	mov rax, qword [new_stack + (200021 * 8)]   ; Load from explicit address
	push rax

	;;printChar
.LABEL135:
	mov rax, 1
	mov rdi, 1
	mov rsi, rsp
	mov rdx, 1
	syscall

	;;NOT IMPLEMENTED NOP
.LABEL136:

	;;NOT IMPLEMENTED NOP
.LABEL137:

	;;push 199711
.LABEL138:
	push 199711

	;;copy 199709
.LABEL139:
	mov rax, qword [new_stack + (199709 * 8)]   ; Load from explicit address
	push rax

	;;add
.LABEL140:
	pop rax
	pop rbx
	add rax, rbx
	push rax

	;;copystack 0
.LABEL141:pop rax
	shl rax, 3
	mov rbx, [new_stack + rax]
	push rbx

	;;NOT IMPLEMENTED NOP
.LABEL142:

	;;push 199912
.LABEL143:
	push 199912

	;;copy 199709
.LABEL144:
	mov rax, qword [new_stack + (199709 * 8)]   ; Load from explicit address
	push rax

	;;add
.LABEL145:
	pop rax
	pop rbx
	add rax, rbx
	push rax

	;;memstack 0
.LABEL146:pop rax
	shl rax, 3
	pop rbx
	mov [new_stack + rax], rbx
	push rbx

	;;push 1
.LABEL147:
	push 1

	;;copy 199709
.LABEL148:
	mov rax, qword [new_stack + (199709 * 8)]   ; Load from explicit address
	push rax

	;;add
.LABEL149:
	pop rax
	pop rbx
	add rax, rbx
	push rax

	;;NOT IMPLEMENTED NOP
.LABEL150:

	;;mem 199709
.LABEL151:
	pop rax
	mov qword [new_stack + (199709 * 8)], rax  ; Store at explicit address
	push rax

	;;push 100
.LABEL152:
	push 100

	;;copy 199709
.LABEL153:
	mov rax, qword [new_stack + (199709 * 8)]   ; Load from explicit address
	push rax

	;;if 0
.LABEL154:
	pop rdi
	pop rsi
	mov rdx, 0
	call COMPARE
	push rax

	;;jmpf
.LABEL155:
	pop rax
	test rax, rax   ; Check if false (0)
	jz .LABEL160

	;;push 0
.LABEL156:
	push 0

	;;NOT IMPLEMENTED NOP
.LABEL157:

	;;mem 199709
.LABEL158:
	pop rax
	mov qword [new_stack + (199709 * 8)], rax  ; Store at explicit address
	push rax

	;;jmp
.LABEL159:
	jmp .LABEL162

	;;NOT IMPLEMENTED NOP
.LABEL160:

	;;jmp
.LABEL161:
	jmp .LABEL136

	;;push 1
.LABEL162:
	push 1

	;;copy 199710
.LABEL163:
	mov rax, qword [new_stack + (199710 * 8)]   ; Load from explicit address
	push rax

	;;add
.LABEL164:
	pop rax
	pop rbx
	add rax, rbx
	push rax

	;;NOT IMPLEMENTED NOP
.LABEL165:

	;;mem 199710
.LABEL166:
	pop rax
	mov qword [new_stack + (199710 * 8)], rax  ; Store at explicit address
	push rax

	;;push 100
.LABEL167:
	push 100

	;;copy 199710
.LABEL168:
	mov rax, qword [new_stack + (199710 * 8)]   ; Load from explicit address
	push rax

	;;if 2
.LABEL169:
	pop rdi
	pop rsi
	mov rdx, 2
	call COMPARE
	push rax

	;;jmpf
.LABEL170:
	pop rax
	test rax, rax   ; Check if false (0)
	jz .LABEL172

	;;jmp
.LABEL171:
	jmp .LABEL174

	;;NOT IMPLEMENTED NOP
.LABEL172:

	;;jmp
.LABEL173:
	jmp .LABEL52

	; END,
.LABEL174:
	mov rax, 60
	mov rdi, 0
	syscall
