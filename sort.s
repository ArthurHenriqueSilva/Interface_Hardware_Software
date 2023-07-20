.intel_syntax noprefix

.section .data
loop_count:         # Variável para armazenar o número de loops externos
    .quad 0

.section .bss
array_size:         # Variável para armazenar o tamanho do array
    .space 4
array:
    .space 100      # Espaço para armazenar o array de inteiros (tamanho máximo de 100 elementos)

.section .text
.global main

main:
    // Registro de pilha
    push rbp
    mov rbp, rsp

    // Abrir o arquivo
    mov rdi, [rsi + 8]          # Argumento argv[1]
    lea rsi, [rip + read_mode]  # Modo de leitura do arquivo
    call fopen@plt
    mov r12, rax                # Salvar o ponteiro do arquivo em r12

    // Ler o número de loops externos
    call read_integer
    mov qword ptr [loop_count], rax

    // Loop externo
.loop_externo:
    // Ler o tamanho do array
    call read_integer
    mov dword ptr [array_size], eax

    // Ler os elementos do array
    mov rsi, array
    mov rdx, array_size
    call read_array

    // Imprimir o array original
    lea rsi, [array]
    mov rdi, format_array
    call printf@plt

    // Ordenar o array
    mov rsi, array
    mov rdx, array_size
    call bubble_sort

    // Imprimir o array ordenado
    lea rsi, [array]
    mov rdi, format_array
    call printf@plt

    // Decrementar o contador de loops externos e verificar se ainda há loops
    dec qword ptr [loop_count]
    jnz .loop_externo

    // Fechar o arquivo
    mov rdi, r12
    call fclose@plt

.exit:
    // Limpar a pilha e sair do programa
    mov rsp, rbp
    pop rbp
    ret

# Função para ler um número inteiro do arquivo
read_integer:
    push rax
    push rcx
    xor rax, rax

.read_loop:
    mov rcx, 10
    mov rdi, r12
    lea rsi, [rsp + 2]  # Buffer temporário na pilha
    mov rdx, 1
    call fread@plt

    mov al, byte ptr [rsp + 2]
    cmp al, 10         # Caractere de nova linha
    je .read_done

    imul rax, rax, 10
    sub al, '0'
    add rax, rax
    add rax, al

    jmp .read_loop

.read_done:
    pop rcx
    pop rax
    ret

# Função para ler um array de inteiros do arquivo
read_array:
    push rax
    push rbx
    push rdi
    push rsi
    push rdx

    xor rbx, rbx          # Índice do array
    mov rsi, rdi          # Ponteiro para o buffer temporário

.read_array_loop:
    call read_integer     # Ler um número inteiro do arquivo
    mov [rsi + rbx*4], eax
    inc rbx
    cmp rbx, rdx
    jl .read_array_loop

    pop rdx
    pop rsi
    pop rdi
    pop rbx
    pop rax
    ret

# Função de ordenação do array usando Bubble Sort
bubble_sort:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi

    mov rbx, rdx           # rbx = tamanho do array
    dec rbx                # Último índice do array
    mov rcx, rbx           # rcx = contador

.outer_loop:
    xor rdx, rdx           # rdx = 0 (marcação para trocas)
    mov rsi, rsi           # rsi = endereço do array
    mov rax, rbx           # rax = último índice do array

.inner_loop:
    mov eax, [rsi + 4]     # eax = array[i]
    mov ebx, [rsi]         # ebx = array[i+1]
    cmp eax, ebx
    jg .swap_elements

.continue_loop:
    add rsi, 4             # Incrementa o ponteiro para o próximo elemento
    loop .inner_loop

    cmp rdx, 0             # Se rdx = 0, o array está ordenado
    jz .done

    dec rbx                # Decrementa o último índice do array
    jmp .outer_loop

.swap_elements:
    mov [rsi], ebx         # array[i] = array[i+1]
    mov [rsi + 4], eax     # array[i+1] = array[i]
    mov rdx, 1             # Marca que houve uma troca
    jmp .continue_loop

.done:
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    ret

.section .rodata
read_mode:
    .string "r"
format_array:
    .string "Array Original: %d"
    .string "\nArray Ordenado: %d\n"
