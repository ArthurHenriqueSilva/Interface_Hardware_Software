#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int matrixSize;
    int matrixIndex;
    int** matrixA;
    int** matrixB;
} MatrixData;

void* matrixMultiply(void* arg) {
    MatrixData* data = (MatrixData*)arg;
    int size = data->matrixSize;
    int index = data->matrixIndex;
    int** matrixA = data->matrixA;
    int** matrixB = data->matrixB;
    int** matrixC = (int**)malloc(size * sizeof(int*));

    for (int i = 0; i < size; i++) {
        matrixC[i] = (int*)malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            matrixC[i][j] = 0;
            for (int k = 0; k < size; k++) {
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }

    char filename[20];
    sprintf(filename, "output%d.txt", index);
    FILE* outputFile = fopen(filename, "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de saída %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(outputFile, "M%d:\n", index);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fprintf(outputFile, "%d ", matrixC[i][j]);
        }
        fprintf(outputFile, "\n");
    }

    fclose(outputFile);
    for (int i = 0; i < size; i++) {
        free(matrixC[i]);
    }
    free(matrixC);

    pthread_exit(NULL);
}

int main() {
    FILE* inputFile = fopen("input.txt", "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada\n");
        exit(EXIT_FAILURE);
    }

    int numMultiplications;
    fscanf(inputFile, "%d", &numMultiplications);

    pthread_t* threads = (pthread_t*)malloc(numMultiplications * sizeof(pthread_t));
    MatrixData* matrixData = (MatrixData*)malloc(numMultiplications * sizeof(MatrixData));

    for (int i = 0; i < numMultiplications; i++) {
        int size;
        fscanf(inputFile, "%d", &size);

        matrixData[i].matrixSize = size;
        matrixData[i].matrixIndex = i + 1;
        matrixData[i].matrixA = (int**)malloc(size * sizeof(int*));
        matrixData[i].matrixB = (int**)malloc(size * sizeof(int*));

        for (int j = 0; j < size; j++) {
            matrixData[i].matrixA[j] = (int*)malloc(size * sizeof(int));
            for (int k = 0; k < size; k++) {
                fscanf(inputFile, "%d", &matrixData[i].matrixA[j][k]);
            }
        }

        for (int j = 0; j < size; j++) {
            matrixData[i].matrixB[j] = (int*)malloc(size * sizeof(int));
            for (int k = 0; k < size; k++) {
                fscanf(inputFile, "%d", &matrixData[i].matrixB[j][k]);
            }
        }

        if (pthread_create(&threads[i], NULL, matrixMultiply, (void*)&matrixData[i]) != 0) {
            fprintf(stderr, "Erro ao criar a thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < numMultiplications; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Erro ao aguardar a thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    fclose(inputFile);
    free(threads);
    for (int i = 0; i < numMultiplications; i++) {
        for (int j = 0; j < matrixData[i].matrixSize; j++) {
            free(matrixData[i].matrixA[j]);
            free(matrixData[i].matrixB[j]);
        }
        free(matrixData[i].matrixA);
        free(matrixData[i].matrixB);
    }
    free(matrixData);

    return 0;
}
