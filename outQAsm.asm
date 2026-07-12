default rel
section .data
	nl db 10
	nll equ $ - nl
	text db "Hello, World!", 10
	len_text equ $ - text
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
	lea rsi, text
	mov rdx, len_text
	call prt ; Print text
	mov rax, 60
	mov rdi, 0
	syscall
	ret
