.intel_syntax noprefix
.section .text
// Main function
.global main
main:
	push rbp
	mov rbp, rsp
	sub rsp, 256
	
	mov rdi, [rsi + 8]
	lea rsi, [rip + readmode]
	call fopen@plt
	mov [rbp - 8], rax
	
	mov rdi, [rbp - 8]
	lea rsi, [rip + intformat]
	lea rdx, [rbp - 16]
	call fscanf@plt
	loop_externo_init:
		mov rcx, 0
	loop_externo:
		cmp rcx, [rbp - 16]
		je done
		lea rdi, [rip + output_index]
		mov rsi, rcx
		mov [rbp - 24], rcx
		call printf@plt
		
		mov rdi, [rbp - 8]
		lea rsi, [rip + intformat]
		lea rdx, [rbp - 32]
		call fscanf@plt
		
		mov r8, [rbp - 32]
		shl r8, 2
		mov rdi, r8
		call malloc@plt
		mov [rip + vector], rax
		
		feed_v_init:
			mov rcx, 0
		feed_v:
			cmp rcx, [rbp - 32]
			je print_v_init
			mov [rbp - 40], rcx
			
			mov rdi, [rbp - 8]
			lea rsi, [rip + intformat]
			lea rdx, [rbp - 48]
			call fscanf@plt
			mov r9, [rbp - 48]
			
			mov rdx, [rip + vector]
			mov [rdx + rcx*4], r9
			
			mov rcx, [rbp - 40]
			inc rcx
			jmp feed_v
		print_v_init:
			mov rcx, 0
		print_v:
			cmp rcx, [rbp - 32]
			je inc_loop_externo
			mov [rbp - 40], rcx
			
			mov rdx, [rip + vector]
			mov r9, [rdx + rcx*4]

			lea rdi, [rip + intformat]
			mov rsi, r9
			call printf@plt
			
			mov rcx, [rbp - 40]
			inc rcx
			jmp print_v
		inc_loop_externo:
			lea rdi, [rip + endl]
			call printf@plt
			mov rcx, [rbp - 24]
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
		.string "[%d] "
	endl:
		.string "\n"
	intformat_endl:
		.string "%d\n"
	space:
		.string " "
