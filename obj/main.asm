default rel
section .data
	nl db 10
	nll equ $ - nl
	vstr_text db "Hello, World!", 10, "x is: "
	len_text equ $ - vstr_text
	vint_x dd 0
section .bss
	bufor resb 12
section .text
	global _start
intToText:
	mov rcx, 0
	mov ebx, 10
.ittLoop:
	mov edx, 0
	div ebx
	add edx, 48
	push rdx
	inc rcx
	cmp eax, 0
	jne .ittLoop
	lea rdi, [rel bufor]
	mov rdx, rcx
	lea rsi, [rel bufor]
.ittLoopWrite:
	pop rax
	mov [rdi], al
	inc rdi
	loop .ittLoopWrite
	inc rdx
	ret
prt:
	mov rax, 1
	mov rdi, 1
	syscall
	ret
_start:
	add dword [rel vint_x], 10 ; Add
	lea rsi, [rel vstr_text]
	mov rdx, len_text
	call prt ; Print string
	sub dword [rel vint_x], 2 ; Sub
	add dword [rel vint_x], 5 ; Add
	mov eax, [rel vint_x]
	call intToText
	call prt ; Print int
	lea rsi, [rel nl]
	mov rdx, nll
	call prt ; Print line
	mov rax, 60
	mov rdi, 0
	syscall
