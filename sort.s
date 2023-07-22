
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
    // argumento 1 para printf
    lea rdi, [rip + output_one_intformat]
    // argumento 2 para printf (pega valor da pilha)
    mov rsi, [rbp -  16]
    call printf@plt
    loop_externo_init:
        mov rcx, 0
    loop_externo:
        cmp rcx, [rbp - 16]
        je done
        lea rdi, [rip + output_one_intformat]
        mov rsi, rcx
        call printf@plt
        inc rcx
        jmp loop_externo
    done:
        //zera  rax
        xor rax, rax
        //reseta pilha
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
