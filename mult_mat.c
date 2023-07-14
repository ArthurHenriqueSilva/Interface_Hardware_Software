#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 100
#define NUM_THREADS 4

int matrixA[SIZE][SIZE];
int matrixB[SIZE][SIZE];
int matrixC[SIZE][SIZE];

// Estrutura de dados para passar informações para as threads
typedef struct {
    int start_row;
    int end_row;
} ThreadData;

// Função executada por cada thread
void *matrixMultiply(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int start_row = data->start_row;
    int end_row = data->end_row;

    // Multiplicação da matriz
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrixC[i][j] = 0;
            for (int k = 0; k < SIZE; k++) {
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];
    int chunk_size = SIZE / NUM_THREADS;

    // Inicialização das matrizes A e B

    // Criação das threads
    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].start_row = i * chunk_size;
        threadData[i].end_row = (i + 1) * chunk_size;
        if (pthread_create(&threads[i], NULL, matrixMultiply, (void *)&threadData[i]) != 0) {
            fprintf(stderr, "Erro ao criar a thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Aguarda o término das threads
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Erro ao aguardar a thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Impressão da matriz resultante
    printf("Matriz C:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", matrixC[i][j]);
        }
        printf("\n");
    }

    return 0;
}
