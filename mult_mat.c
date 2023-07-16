#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int row;
    int col;
    int size;
    double **matrix1;
    double **matrix2;
    double **result;
} ThreadArgs;

void *multiply_matrices(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    int row = args->row;
    int col = args->col;
    int size = args->size;
    double **matrix1 = args->matrix1;
    double **matrix2 = args->matrix2;
    double **result = args->result;

    for (int k = 0; k < size; k++) {
        result[row][col] += matrix1[row][k] * matrix2[k][col];
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    char *input_file = argv[1];

    // Leitura das matrizes de entrada
    FILE *input = fopen(input_file, "r");
    int num_matrices;
    fscanf(input, "%d", &num_matrices);

    // Criação das matrizes
    double ***matrices = (double ***)malloc(num_matrices * sizeof(double **));
    for (int i = 0; i < num_matrices; i++) {
        int rows, cols;
        fscanf(input, "%d %d", &rows, &cols);
        matrices[i] = (double **)malloc(rows * sizeof(double *));
        for (int j = 0; j < rows; j++) {
            matrices[i][j] = (double *)malloc(cols * sizeof(double));
            for (int k = 0; k < cols; k++) {
                fscanf(input, "%lf", &matrices[i][j][k]);
            }
        }
    }
    fclose(input);

    // Multiplicação das matrizes
    int num_rows1 = 0, num_cols1 = 0, num_rows2 = 0, num_cols2 = 0;
    double **matrix1 = matrices[0];
    double **matrix2 = matrices[1];
    double **result = (double **)malloc(num_rows1 * sizeof(double *));
    pthread_t *threads = (pthread_t *)malloc(num_rows1 * num_cols2 * sizeof(pthread_t));
    ThreadArgs *args = (ThreadArgs *)malloc(num_rows1 * num_cols2 * sizeof(ThreadArgs));

    for (int i = 0; i < num_rows1; i++) {
        result[i] = (double *)calloc(num_cols2, sizeof(double));
        for (int j = 0; j < num_cols2; j++) {
            args[i * num_cols2 + j].row = i;
            args[i * num_cols2 + j].col = j;
            args[i * num_cols2 + j].size = num_cols1;
            args[i * num_cols2 + j].matrix1 = matrix1;
            args[i * num_cols2 + j].matrix2 = matrix2;
            args[i * num_cols2 + j].result = result;

            pthread_create(&threads[i * num_cols2 + j], NULL, multiply_matrices, (void *)&args[i * num_cols2 + j]);
        }
    }

    // Aguarda a finalização de todas as threads
    for (int i = 0; i < num_rows1 * num_cols2; i++) {
        pthread_join(threads[i], NULL);
    }

    // Imprime os resultados na tela
    for (int i = 0; i < num_rows1; i++) {
        printf("M%d:\n", i);
        for (int j = 0; j < num_cols2; j++) {
            printf("  %.2lf", result[i][j]);
        }
        printf("\n");
    }

    // Libera a memória alocada
    for (int i = 0; i < num_matrices; i++) {
        int rows = num_rows1;
        for (int j = 0; j < rows; j++) {
            free(matrices[i][j]);
        }
        free(matrices[i]);
    }
    free(matrices);
    for (int i = 0; i < num_rows1; i++) {
        free(result[i]);
    }
    free(result);
    free(threads);
    free(args);

    return 0;
}
