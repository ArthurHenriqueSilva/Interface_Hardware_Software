#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int matrixIndex;
    int numRowsA;
    int numColsA;
    int numColsB;
    int** matrixA;
    int** matrixB;
    char* outputFilename;
} MatrixData;

void* matrixMultiply(void* arg) {
    MatrixData* data = (MatrixData*)arg;
    int index = data->matrixIndex;
    int numRowsA = data->numRowsA;
    int numColsA = data->numColsA;
    int numColsB = data->numColsB;
    int** matrixA = data->matrixA;
    int** matrixB = data->matrixB;
    char* outputFilename = data->outputFilename;
    int** matrixC = (int**)malloc(numRowsA * sizeof(int*));

    for (int i = 0; i < numRowsA; i++) {
        matrixC[i] = (int*)malloc(numColsB * sizeof(int));
        for (int j = 0; j < numColsB; j++) {
            matrixC[i][j] = 0;
            for (int k = 0; k < numColsA; k++) {
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }

    FILE* outputFile = fopen(outputFilename, "a");
    if (outputFile == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de saída %s\n", outputFilename);
        exit(EXIT_FAILURE);
    }

    fprintf(outputFile, "M%d:\n", index);
    for (int i = 0; i < numRowsA; i++) {
        for (int j = 0; j < numColsB; j++) {
            fprintf(outputFile, "%d ", matrixC[i][j]);
        }
        fprintf(outputFile, "\n");
    }

    fclose(outputFile);
    for (int i = 0; i < numRowsA; i++) {
        free(matrixC[i]);
    }
    free(matrixC);

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso incorreto. Sintaxe correta: %s arquivo_entrada arquivo_saida\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* inputFilename = argv[1];
    char* outputFilename = argv[2];

    FILE* inputFile = fopen(inputFilename, "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada: %s\n", inputFilename);
        exit(EXIT_FAILURE);
    }

    int numMultiplications;
    fscanf(inputFile, "%d", &numMultiplications);

    pthread_t* threads = (pthread_t*)malloc(numMultiplications * sizeof(pthread_t));
    MatrixData* matrixData = (MatrixData*)malloc(numMultiplications * sizeof(MatrixData));

    for (int i = 0; i < numMultiplications; i++) {
        int numRowsA, numColsA, numColsB;
        fscanf(inputFile, "%d %d %d", &numRowsA, &numColsA, &numColsB);

        matrixData[i].matrixIndex = i + 1;
        matrixData[i].numRowsA = numRowsA;
        matrixData[i].numColsA = numColsA;
        matrixData[i].numColsB = numColsB;
        matrixData[i].matrixA = (int**)malloc(numRowsA * sizeof(int*));
        matrixData[i].matrixB = (int**)malloc(numColsA * sizeof(int*));
        matrixData[i].outputFilename = outputFilename;

        for (int j = 0; j < numRowsA; j++) {
            matrixData[i].matrixA[j] = (int*)malloc(numColsA * sizeof(int));
            for (int k = 0; k < numColsA; k++) {
                fscanf(inputFile, "%d", &matrixData[i].matrixA[j][k]);
            }
        }

        for (int j = 0; j < numColsA; j++) {
            matrixData[i].matrixB[j] = (int*)malloc(numColsB * sizeof(int));
            for (int k = 0; k < numColsB; k++) {
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
        for (int j = 0; j < matrixData[i].numRowsA; j++) {
            free(matrixData[i].matrixA[j]);
        }
        free(matrixData[i].matrixA);
        for (int j = 0; j < matrixData[i].numColsA; j++) {
            free(matrixData[i].matrixB[j]);
        }
        free(matrixData[i].matrixB);
    }
    free(matrixData);

    return 0;
}
