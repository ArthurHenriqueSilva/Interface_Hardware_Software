#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int start_row;
    int end_row;
    double **matrix1;
    double **matrix2;
    double **result;
} ThreadArgs;

void *multiply_matrices(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    int start_row = args->start_row;
    int end_row = args->end_row;
    double **matrix1 = args->matrix1;
    double **matrix2 = args->matrix2;
    double **result = args->result;

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < cols2; j++) {
            for (int k = 0; k < cols1; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    char *input_file1 = argv[1];
    char *input_file2 = argv[2];
    char *output_file = "output.txt";

    // Leitura das matrizes de entrada
    // Código para ler as dimensões e preencher as matrizes omitido por simplicidade

    // Criação das threads
    int num_threads = 4; // Número de threads a serem usadas
    pthread_t threads[num_threads];
    ThreadArgs args[num_threads];

    int block_size = rows1 / num_threads;
    int remaining_rows = rows1 % num_threads;
    int start_row = 0;

    for (int i = 0; i < num_threads; i++) {
        int end_row = start_row + block_size;
        if (i < remaining_rows) {
            end_row++;
        }
        
        args[i].start_row = start_row;
        args[i].end_row = end_row;
        args[i].matrix1 = matrix1;
        args[i].matrix2 = matrix2;
        args[i].result = result;

        pthread_create(&threads[i], NULL, multiply_matrices, (void *)&args[i]);

        start_row = end_row;
    }

    // Aguarda a finalização de todas as threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Escrita da matriz resultante no arquivo de saída
    // Código para escrever a matriz resultante no arquivo omitido por simplicidade

    return 0;
}
