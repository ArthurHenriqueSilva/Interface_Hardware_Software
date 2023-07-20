.intel_syntax noprefix

// Variável para armazenar o número de loops externos
.section .data
loop_count:
    .quad 0

// Variável para armazenar o tamanho do array
.section .bss
array_size:
    .space 4

// Espaço para armazenar o array de inteiros (tamanho máximo de 100 elementos)
array:
    .space 100

.section .text
.global main

// Função principal
main:
    // Registro de pilha
    push rbp
    mov rbp, rsp

    // Abrir o arquivo
    mov rdi, [rsi + 8]          
    lea rsi, [rip + read_mode]  
    call fopen@plt
    mov r12, rax                

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

// Fim do programa
.exit:
    // Limpar a pilha e sair do programa
    mov rsp, rbp
    pop rbp
    ret

// Função para ler um número inteiro do arquivo
read_integer:
    push rax
    push rcx
    xor rax, rax

.read_loop:
    mov rcx, 10
    mov rdi, r12
    lea rsi, [rsp + 2]  
    mov rdx, 1
    call fread@plt

    mov al, byte ptr [rsp + 2]
    cmp al, 10         
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

// Função para ler um array de inteiros do arquivo
read_array:
    push rax
    push rbx
    push rdi
    push rsi
    push rdx

    xor rbx, rbx          
    mov rsi, rdi          

.read_array_loop:
    call read_integer     
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

// Função de ordenação do array usando Bubble Sort
bubble_sort:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi

    mov rbx, rdx           
    dec rbx                
    mov rcx, rbx           

.outer_loop:
    xor rdx, rdx           
    mov rsi, rsi           
    mov rax, rbx           

.inner_loop:
    mov eax, [rsi + 4]     
    mov ebx, [rsi]         
    cmp eax, ebx
    jg .swap_elements

.continue_loop:
    add rsi, 4             
    loop .inner_loop

    cmp rdx, 0            
    jz .done

    dec rbx                
    jmp .outer_loop

.swap_elements:
    mov eax, [rsi]         
    mov ebx, [rsi + 4]     
    mov [rsi], ebx         
    mov [rsi + 4], eax     
    mov rdx, 1             
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
