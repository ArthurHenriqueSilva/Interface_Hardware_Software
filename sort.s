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
			mov r19, [rbp - 56]
			mov [rdx + rcx*4], r19
			mov rcx, [rbp - 48]
			inc rcx
			jmp feed_vector
		inc_loop_ext:
			sort_vector_init:
				mov rcx, 0
                                mov r15, [rbp - 40]
				mov r18, [rbp - 40]
				sub r18, 1
			sort_vector:
                                cmp rcx, r18
                                mov [rbp - 64], rcx
				je print_vector_init
                                mov r16, r15
                                sort_inner_loop_init:
                                        mov r17, r15
                                        inc r17
					mov rcx, r17
				sort_inner_loop:
					cmp r17, 15
					je inc_sort
					mov rdx, [rip + vector]
					mov r18, [rdx + rcx*4]
                                        mov r19, [rdx + r16*4]
					cmp r18, r19
					jg part_2_inner
					mov r16, 17
				part_2_inner:
					cmp r16, r15
					je inc_inner
				inc_inner_loop:
					inc rcx
					jmp sort_inner_loop
				inc_sort:
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
