default rel

section .data
    nl db 10 ; new line
    nll equ $ - nl

    var_a dd 10 ; int a 10

    pl1t db "a is " ; println a is v(a)
    pl1l equ $ - pl1t

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
;     mov byte [rdi], 10
    inc rdx
    ret

; === intToText =======================================================
; Input:  EAX - Number
; Output: RSI - Text
; Output: RDX - Text Length
; === prt =============================================================
; Input: RSI - Text
; Input: RDX - Text Lenght
; =====================================================================
prt:
    mov rax, 1
    mov rdi, 1
    syscall
    ret
; =====================================================================
; data
;     int a 10
; end
;
; add a 13
; sub a 3
; mul a 2
; div a 8
; println a is v(a)
_start:
    mov eax, [rel var_a]
    add eax, 13
    mov [rel var_a], eax ; add a 6

    mov eax, [rel var_a]
    sub eax, 3
    mov [rel var_a], eax ; sub a 3

    mov eax, [rel var_a]
    imul eax, 2
    mov [rel var_a], eax ; mul a 2

    mov eax, [rel var_a]
    cdq
    mov ecx, 8
    idiv ecx
    mov [rel var_a], eax ; div a 8

    lea rsi, [rel pl1t]
    mov rdx, pl1l
    call prt ; println a is

    mov eax, [rel var_a]
    call intToText
    call prt ; v(a)

    lea rsi, [rel nl]
    mov rdx, nll
    call prt ; new line

    mov rax, 60
    mov rdi, 0
    syscall
