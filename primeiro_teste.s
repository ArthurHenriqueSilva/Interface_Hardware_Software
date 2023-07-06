.intel_syntax noprefix


.section .text
//Main function
.global main
main:
	//registro de pilha
	push rbp
	mov rbp, rsp
	
	//ler nome do arquivo_entrada(argv[1])
	mov r12, [rsi+8] //guarda valor de argv[1]
	lea rsi, [rip + leitura] // argumento 1 para fopen
	mov rdi, r12 //argumento 2 para fopen
	call fopen@plt
	mov r13, rax // guarda ponteiro do arquivo em r13
	mov r14, [r13 + 4]  //variavel de loop externo(int) (primeira linha do arquivo)

	
	
	
.section .rodata

leitura:
	.string "r\n"
