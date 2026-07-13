; = > QAsm v. 0.0.21
default rel
; = > MACROS
%macro RT 2
	mov eax, %2
	mov [rel qai%1], eax
%endmacro
; = > END MACROS
section .data
	; QAsm data:
	qaia dd 0 ; QAsm int A
	qaib dd 0 ; QAsm int B
	; User data:
	text db "Hello, World!", 10
	len_text equ $ - text
section .bss
	itt_bfr resb 21
	prtbh_bfr resb 64
section .text
	global _start
; = = = INT TO TEXT = = =
; RT a, [value]
; call intToText
; call prt
intToText:
	mov rcx, 0
	mov ebx, 10
	xor rdx, rdx
.ittLoop:
	mov edx, 0
	div ebx
	add edx, 48
	push rdx
	inc rcx
	cmp eax, 0
	jne .ittLoop
	lea rdi, [rel itt_bfr]
	mov rdx, rcx
	lea rsi, [rel itt_bfr]
.ittLoopWrite:
	pop rax
	mov [rdi], al
	inc rdi
	loop .ittLoopWrite
	inc rdx
	ret
; = = = PRINT = = =
; mov rsi, [text]
; mov rdx, [text length]
; call prt
prt:
	mov rax, 1
	mov rdi, 1
	syscall
	ret
prtln:
	mov rsi, 10
	mov rdx, 1
	call prt
	ret
; = = = PRINT BINARY = = =
	; RT a, [value]
	; RT b, [length]
	; call prtB
prtB:
	mov eax, [rel qaia]
	lea rdi, [rel prtbh_bfr]
	mov rcx, 32
	sub rcx, [rel qaib]
	shl eax, cl
	mov rcx, [rel qaib]
	lea rdi, [rel prtbh_bfr]
.prtBloop:
	mov edx, eax
	shr edx, 31
	add dl, '0'
	mov [rdi], dl
	inc rdi
	shl eax, 1
	loop .prtBloop
	mov rsi, prtbh_bfr
	mov rdx, [rel qaib]
	call prt
	ret
; = = = PRINT HEX = = =
	; RT a, [value]
	; RT b, [length]
	; call prtH
prtH:
	mov eax, [rel qaia]
	lea rdi, [rel prtbh_bfr]
	mov rcx, 8
	sub rcx, [rel qaib]
	shl rcx, 2
	shl eax, cl
	mov rcx, [rel qaib]
.prtHloop:
	rol eax, 4
	mov edx, eax
	and edx, 0xF
	cmp dl, 9
	jbe .prtHdigit
	add dl, 7
.prtHdigit:
	add dl, '0'
	mov [rdi], dl
	inc rdi
	loop .prtHloop
	mov rsi, [rel prtbh_bfr]
	mov rdx, [rel qaib]
	call prt
	ret
_start:
	lea rsi, text
	mov rdx, len_text
	call prt ; Print text
	mov rax, 60
	mov rdi, 0
	syscall
	ret
