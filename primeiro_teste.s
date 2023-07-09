.intel_syntax noprefix
gcc -Wall primeiro_teste.s -o primeiro_teste.bin

.section .text
//Main function
.global main
main:
	//registro de pilha
	push rbp
	mov rbp, rsp
	
	//ler nome do arquivo_entrada(argv[1])
	mov r12, rsi //guardar argv
	mov r13, [rsi+8] //guarda valor de argv[1]
	lea rdi, [rip + leitura] // argumento 1 para fopen
	mov rsi, r13 //argumento 2 para fopen

    //abrir arquivo_entrada
	call fopen@plt
	mov r14, rax // guarda ponteiro do arquivo
	mov rdi, r14 //argumento 1 para fscanf
	lea rsi, [rip + inteiro] //argumento 2 para fscanf
	call fscanf@plt
	mov r15, rax // guarda ponteiro de dentro do arquivo
	mov r16, 1 //ancora para leitura (ptr + r16*4)
 	shl r16, 2 // r16*4
  	mov r17, [r15 + r16] // r17 = file[r16], leitura primeiro inteiro do arquivo
   	lea rsi, [rip + inteiro] // argumento 1 printf
    mov rdi, rdi
    call printf@plt
  	
	


	
	
	
.section .rodata
leitura:
	.string "r\n"
inteiro:
	.string "%d\n"
