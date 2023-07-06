.intel_syntax noprefix

.section .text
.global main
main:
    //salvar registro de pilha
    push rbp
    mov rbp, rsp
    //reservar espaco para o nome do arquivo
    sub rsp, 100

    //verificar argumentos
    mod edi, 1
    mov esi, dword ptr [rbp + 8]//endereco do argumento na pilha
    call arg_check

    //reposiciona o nome do arquivo na pilha
    mov rdi, rsp
    mov rsi, dword ptr [rbp + 16]
    call strcpy@plt

    //abrir arquivo de entrada
    mov edi, arquivo_entrada
    mov esi, 0 // modo leitura para funcao fopen
    call fopen@plt
    mov r9, rax // ponteiro para o arquivo em r9


    //ler e imprimir cada linha do arquivo
    loop_leitura_print:
        //ler linha do arquivo
        lea rdi, [rsp] // endereço da linha
        mov rsi, tamanho_linha
        mov rdx, r9
        call fgets@plt

        //verificar EOF
        cmp rax, 0
        je fim_loop

        //imprimir linha lida
        lea rdi, [rsp]
        call prinf@plt
        jmp loop_leitura_print
    
    fim_loop:
        mov rdi, r9
        call fclose@plt
    
    mov rsp, rbp
    pop rbp
    xor eax, eax // retorno 0
    ret


//BSS segment
.section .bss
arg_check:
    cmp edi, dword ptr [rsi - 8]
    jge arg_ok
    jmp arg_falha

arg_falha:
    mov eax, 1
    mov edi, 1
    xor esi, esi
    mov edx, 0
    call write@plt
    jmp exit

arg_ok:
    xor eax, eax
    ret
