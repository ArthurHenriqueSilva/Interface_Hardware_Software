.intel_syntax noprefix
.section .text
.global main
main:
	push rbp
	mov rbp, rsp
	sub rsp, 128
	
	mov r12, rsi
	mov r13, [rsi+8]
	
	mov rdi, r13
	lea rsi, [rip + readmode]
	call fopen@plt
	mov [rbp - 32], rax
	
	mov rdi, [rbp - 32]
	lea rsi, [rip + intformat]
	lea rdx, [rbp - 16]
	call fscanf@plt
	
	xor r9, r9
	mov r10, [rbp - 16]
	
	loop_externo:
		cmp r9, r10
		je done
		
		lea rdi, [rip + output_index]
		mov rsi, r9
		call printf@plt
		
		mov rdi, [rbp - 32]
		lea rsi, [rip + intformat]
		lea rdx, [rbp - 40]
		call fscanf@plt
		mov r14, [rbp - 40]
		shl r14, 2
		
		mov rdi, r14
		call malloc@plt
		mov [rip + vector], rax
		
		xor r9, r9
		feed_vector:
			cmp r9, r14
			je sort_vector
			
			mov rdi, [rbp - 32]
			lea rsi, [rip + intformat]
			lea rdx, [rbp - 56]
			call fscanf@plt
			
			mov rdx, [rip + vector]
			mov [rdx + r9*4], [rbp - 56]
			inc r9
			jmp feed_vector
		
		sort_vector:
			mov rdx, [rip + vector]
			mov r8, r14
		
		sort_outer_loop:
			xor rbx, rbx
			mov rcx, 1
			
		sort_inner_loop:
			mov r9, [rdx + rcx*8]
			mov r10, [rdx + rcx*8 - 8]
			cmp r10, r9
			jle sort_continue
			
			mov [rdx + rcx*8], r10
			mov [rdx + rcx*8 - 8], r9
			mov rbx, 1
		
		sort_continue:
			inc rcx
			cmp rcx, r8
			jnz sort_inner_loop
			test rbx, rbx
			jnz sort_outer_loop
			
			jmp print_vector
		
		print_vector:
			cmp r9, r14
			je inc_loop_externo
			
			mov rdx, [rip + vector]
			lea rdi, [rip + intformat]
			mov rsi, [rdx + r9*4]
			call printf@plt
			
			inc r9
			jmp print_vector
		
		inc_loop_externo:
			lea rdi, [rip + endl]
			call printf@plt
			
			mov r9, [rbp - 8]
			inc r9
			jmp loop_externo
	
	done:
		xor rax, rax
		mov rsp, rbp
		pop rbp
		ret

.section .bss
vector:
.resq 1

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
