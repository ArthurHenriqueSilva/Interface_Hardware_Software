.intel_syntax noprefix
.section .text
// Main function
.global main
main:
	// registro de pilha
	push rbp
	mov rbp, rsp
	// reserva memoria na pilha
	sub rsp, 128
	// ler nome do arquivo_entrada(argv[1])
	// guardar argv
	mov r12, rsi
	// guarda valor de argv[1]
	mov r13, [rsi+8]
	//mov r14, [rsi+16]
	// argumento 1 para fopen
	mov rdi, r13
	// argumento 2 para fopen
	lea rsi, [rip + readmode]
	// abrir arquivo_entrada
	call fopen@plt
	// guarda ponteiro do arquivo
	mov [rbp - 32], rax
        
	//mov rdi, r14
        //lea rsi, [rip + appendmode]
	//call fopen@plt
        //mov [rbp - 628], rax
	// argumento 1 para fscanf
	mov rdi, [rbp - 32]
	// argumento 2 para fscanf
	lea rsi, [rip + intformat]
	// argumento 3 para fscanf
	lea rdx, [rbp - 16]
	call fscanf@plt
	loop_externo_init:
	    // i = 0
	    mov rcx, 0
        loop_externo:
	    // i < r15
	    cmp rcx, [rbp - 16]
            je done

            lea rdi, [rip + output_index]
            mov rsi, rcx
            mov [rbp - 8], rcx
	    call printf@plt

            mov rdi, [rbp - 32]
            lea rsi, [rip + intformat]
            lea rdx, [rbp - 40]
            call fscanf@plt
	    mov r14, [rbp - 40]
            shl r14, 2
            //mov [rbp - 40], r14
            //lea rdi, [rip + intformat]
            //mov rsi, [rbp - 40]
            //call printf@plt
            mov rdi, r14
            call malloc@plt
            mov [rip + vector], rax
            feed_vector_init:
                mov rcx, 0
            feed_vector:
                cmp rcx, [rbp - 40]
                mov [rbp - 48], rcx
                je inc_loop_ext
                mov rdi, [rbp - 32]
                lea rsi, [rip + intformat]
                lea rdx, [rbp - 56]
                call fscanf@plt
                
                mov rdx, [rip + vector]
                mov [rdx + rcx*4], [rbp - 56]
                mov rcx, [rbp - 48]
                inc rcx
                jmp feed_vector
            inc_loop_ext:
                sort_vector_init:
                    mov rcx, 0
                sort_vector:
                    mov [rbp - 64], rcx
                    mov rcx, [rbp - 64]
                    inc rcx
                    jmp sort_vector
                print_vector_init:
                    mov rcx, 0
                print_vector:
                    cmp rcx, [rbp - 40]
                    mov [rbp - 72], rcx
                    je inc_loop_externo
                    mov rdx, [rip + vector]
                    lea rdi, [rip + intformat]
                    mov rsi, [rdx + rcx*4]
                    call printf@plt
                    mov rcx, [rbp - 72]
                    inc rcx
                    jmp print_vector
                inc_lopp_externo:
                    lea rdi, [rip + endl]
                    call printf@plt
                    mov rcx, [rbp - 8]
                    inc rcx
                    jmp loop_externo
        done:
	    xor rax, rax
	    mov rsp, rbp
 	    pop rbp
	    ret

.section .bss
vector:
    .8byte
.section .rodata  
appendmode:
    .string "a"
readmode:
    .string "r"
intformat:
    .string "%d"
output_one_intformat:
    .string "%d\n"
output_index:
   .string "[%d]"
endl:
   .string "\n"
intformat_endl:
   .string "%d\n"
