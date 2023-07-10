.intel_syntax noprefix

.section .text
// Main function
.global main
main:
    // registro de pilha
    push rbp
    mov rbp, rsp
    //reserva memoria na pilha
    sub rsp, 16

    
    // ler nome do arquivo_entrada(argv[1])
    // guardar argv
    mov r12, rsi
    // guarda valor de argv[1]
    mov r13, [rsi+8]
    // argumento 1 para fopen
    lea rdi, [rip + leitura] 
    // argumento 2 para fopen
    mov rsi, r13 
    

    // abrir arquivo_entrada
    call fopen@plt
    // guarda ponteiro do arquivo
    mov r14, rax
    // argumento 1 para fscanf
    mov rdi, r14
    // argumento 2 para fscanf
    lea rsi, [rip + inteiro] 
    call fscanf@plt
    // guarda ponteiro de dentro do arquivo. r15 = ptr
    mov r15, rax
    // ancora para leitura (ptr + r16*4)
    mov r16, 1
    // r16*4
    shl r16, 2
    // r17 = file[r16], leitura primeiro inteiro do arquivo
    mov r17, [r15 + r16]
    // argumento 1 printf
    mov rdi, r17
    //argumento 2 printf
    lea rsi, [rip + inteiro] 
    call printf@plt



.section .rodata
leitura:
    .string "r\n"
inteiro:
    .string "%d\n"
