.intel_syntax noprefix

.section .text
// Main function
.global main
main:
    // registro de pilha
    push rbp
    mov rbp, rsp
    // reserva memoria na pilha
    sub rsp, 16    
    // ler nome do arquivo_entrada(argv[1])
    // guardar argv
    mov r12, rsi
    // guarda valor de argv[1]
    mov r13, [rsi+8]
    // argumento 1 para fopen
    mov rdi, r13
    // argumento 2 para fopen
    lea rsi, [rip + readmode] 
    // abrir arquivo_entrada
    call fopen@plt
    // guarda ponteiro do arquivo
    mov r14, rax
    // argumento 1 para fscanf
    mov rdi, r14
    // argumento 2 para fscanf
    lea rsi, [rip + intformat]
    // argumento 3 para fscanf (endereço da variável onde o valor será armazenado)
    lea rdx, [rbp - 8]
    call fscanf@plt
    mov r15, rdx

    // argumento 1 printf
    mov rsi, [r15]  // Correção: removendo o offset de 4 bytes para obter o valor lido corretamente.
    // argumento 2 printf
    lea rdi, [rip + intformat]
    call printf@plt
    xor rax, rax
    mov rsp, rbp
    pop rbp
    ret

.section .rodata
readmode:
    .string "r\n"
intformat:
    .string "%d\n"
