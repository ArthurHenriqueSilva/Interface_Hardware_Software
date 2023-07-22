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
    // argumento 3 para fscanf
    lea rdx, [rbp - 16]
    call fscanf@plt
    lea rdi, [rip + output_one_intformat]
    mov rsi, [rbp - 16]
    call printf@plt
    mov r15, [rbp - 16]
    // for(i = 0; i < r15; i++)
    loop_externo_init:
        // i = 0
        mov rcx, 0
    loop_externo:
        // i < r15
        cmp rcx, r15
        je done

        // Imprime o valor de rcx
        lea rdi, [rip + output_one_intformat]
        mov rsi, rcx
        call printf@plt

        // i++
        inc rcx
        jmp loop_externo

    done:
        xor rax, rax
        mov rsp, rbp
        pop rbp
        ret


.section .rodata  
readmode:
    .string "r"
intformat:
    .string "%d"
output_one_intformat:
    .string "%d\n"
