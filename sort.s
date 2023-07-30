.intel_syntax noprefix
.section .text
// Main function
.global main
main:
		push rbp
		mov rbp, rsp
		sub rsp, 128
		mov r8, [rsi+8]
		mov rdi, r8
		lea rsi, [rip + readmode]
		call fopen@plt
		mov [rbp - 32], rax
		mov rdi, [rbp - 32]
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
		mov [rbp - 8], rcx
		call printf@plt

		mov rdi, [rbp - 32]
		lea rsi, [rip + intformat]
		lea rdx, [rbp - 40]
		call fscanf@plt
		mov r9, [rbp - 40]
		shl r9, 2

		mov rdi, r9
		call malloc@plt
		mov [rip + vector], rax
		feed_vector_init:
			mov rcx, 0
		feed_vector:
			cmp rcx, [rbp - 40]
			mov [rbp - 48], rcx
			je print_vector_init
			mov rdi, [rbp - 32]
			lea rsi, [rip + intformat]
			lea rdx, [rbp - 56]
			call fscanf@plt
			mov rdx, [rip + vector]
			mov r10, [rbp - 56]
			mov [rdx + rcx*4], r10
			lea rdi, [rip + intformat]
			mov rsi, [rdx + rcx*4]
			call printf@plt
			lea rdi, [rip + space]
			call printf@plt
			mov rcx, [rbp - 48]
			inc rcx
			jmp feed_vector
		print_vector_init:
			mov rcx, 0
		print_vector:
			cmp rcx, [rbp - 40]
			je inc_loop_ext
			mov [rbp - 64], rcx
			lea rdi, [rip + intformat]
			lea rdx, [rip + vector]
			mov rsi, [rdx + rcx*4]
			call printf@plt
			lea rdi, [rip + space]
			call printf@plt
			mov rcx, [rbp - 64]
			inc rcx
			jmp print_vector
		inc_loop_ext:
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
		.string "[%d] "
	endl:
		.string "\n"
	intformat_endl:
		.string "%d\n"
	space:
		.string " "
