.intel_syntax noprefix

.section .data
mensagem_erro:
    .asciz "Argumento inválido\n"
tamanho_linha equ 256  // Defina o tamanho máximo esperado de uma linha

.section .text
.global main
main:
    // salvar registro de pilha
    push rbp
    mov rbp, rsp
    // reservar espaço para o nome do arquivo
    sub rsp, 100

    // verificar argumentos
    mov edi, 1
    mov esi, dword ptr [rbp + 8] // endereço do argumento na pilha
    call arg_check

    // reposicionar o nome do arquivo na pilha
    mov rdi, rsp
    mov rsi, qword ptr [rbp + 16] // endereço do arquivo_entrada (argv[1]) na pilha
    call strcpy@plt

    // abrir arquivo de entrada
    mov edi, rsp // nome do arquivo
    mov esi, 0 // modo leitura para a função fopen
    call fopen@plt
    mov r9, rax // ponteiro para o arquivo em r9

    // ler e imprimir cada linha do arquivo
    loop_leitura_print:
        // ler linha do arquivo
        lea rdi, [rsp] // endereço da linha
        mov rsi, tamanho_linha
        mov rdx, r9
        call fgets@plt

        // verificar EOF
        cmp rax, 0
        je fim_loop

        // determinar o tamanho da linha
        lea rdi, [rsp]
        call strlen

        // salvar tamanho da linha
        mov r10, rax

        // imprimir linha lida
        lea rdi, [rsp]
        call printf@plt

        // restaurar o ponteiro de pilha
        add rsp, r10

        jmp loop_leitura_print

    fim_loop:
        mov rdi, r9
        call fclose@plt

    // retornar posição da pilha
    mov rsp, rbp
    pop rbp
    xor eax, eax // retorno 0
    ret

// BSS segment
.section .bss
.global arg_check
arg_check:
    cmp edi, dword ptr [rsi - 8] // comparação de argumento válido
    jge arg_ok // if ok
    jmp arg_falha // if false

arg_falha:
    mov edi, 1 // indicar erro
    lea rsi, [rip + mensagem_erro]
    mov edx, tamanho_mensagem_erro
    call write@plt
    jmp exit

arg_ok:
    xor eax, eax // zera eax
    ret
