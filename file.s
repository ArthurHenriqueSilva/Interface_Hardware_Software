
.intel_syntax noprefix

.section .text
// Main function
.global main
main:
    // registro de pilha
    prologue:
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
    lea rsi, [rip + leitura] 
    // abrir arquivo_entrada
    call fopen@plt
    // guarda ponteiro do arquivo
    mov r14, rax
    // argumento 1 para fscanf
    mov rdi, r14
    // argumento 2 para fscanf
    lea rsi, [rip + inteiro]
    lea rdx, [rbp - 16]
    call fscanf@plt
    // guarda ponteiro de dentro do arquivo. r15 = ptr
    mov r15, rdx
    // ancora para leitura (ptr + r16*4)
    
    // argumento 1 printf
    lea rsi, [r15 + 4]   
    //argumento 2 printf
    lea rdi, [rip + inteiro] 
    call printf@plt
    epilogue:
        mov rsp, rbp
        pop rbp
        ret
        
.section .rodata
leitura:
    .string "r\n"
inteiro:
    .string "%d\n"
