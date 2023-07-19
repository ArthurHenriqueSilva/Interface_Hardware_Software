#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int n1, m1, n2, m2;
    double** matriz1;
    double** matriz2;
    double** result;
} MatrixData;

typedef struct {
    int start_row, end_row, start_col, end_col;
    MatrixData* md;
} ThreadData;

void* mult_block(void* args) {
    ThreadData* td = (ThreadData*)args;
    int n = td->md->n1;
    int m = td->md->m2;

    for (int i = td->start_row; i < td->end_row; i++) {
        for (int j = td->start_col; j < td->end_col; j++) {
            double r = 0;
            for (int k = 0; k < td->md->m1; k++) {
                r += td->md->matriz1[i][k] * td->md->matriz2[k][j];
            }
            td->md->result[i][j] = r;
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    printf("Programa comecou!\n");
    FILE* input = fopen(argv[1], "r");
    if (input == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    int num_iter;
    fscanf(input, "%d", &num_iter);
    printf("num_iter = %d\n", num_iter);

    for (int c = 0; c < num_iter; c++) {
        MatrixData md;
        fscanf(input, "%d %d", &md.n1, &md.m1);
        printf("%d %d\n", md.n1, md.m1);
        md.matriz1 = (double**)malloc(md.n1 * sizeof(double*));
        for (int i = 0; i < md.n1; i++) {
            md.matriz1[i] = (double*)malloc(md.m1 * sizeof(double));
            for (int j = 0; j < md.m1; j++) {
                fscanf(input, "%lf", &md.matriz1[i][j]);
            }
        }

        fscanf(input, "%d %d", &md.n2, &md.m2);
        printf("%d %d\n", md.n2, md.m2);
        md.matriz2 = (double**)malloc(md.n2 * sizeof(double*));
        for (int i = 0; i < md.n2; i++) {
            md.matriz2[i] = (double*)malloc(md.m2 * sizeof(double));
            for (int j = 0; j < md.m2; j++) {
                fscanf(input, "%lf", &md.matriz2[i][j]);
            }
        }

        int n = md.n1;
        int m = md.m2;
        md.result = (double**)malloc(n * sizeof(double*));
        for (int i = 0; i < n; i++) {
            md.result[i] = (double*)malloc(m * sizeof(double));
        }

        // Número eficiente de threads (número de núcleos do processador)
        int num_threads = sysconf(_SC_NPROCESSORS_ONLN);
        printf("num_threads %d\n", num_threads);

        // Aloca espaço para as threads e dados específicos de cada thread
        pthread_t* threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
        ThreadData* td = (ThreadData*)malloc(num_threads * sizeof(ThreadData));

        // Calcula o tamanho dos blocos para cada thread
        int block_size = n / num_threads;
        int remaining_blocks = n % num_threads;

        // Criação das threads
        int start_row = 0;
        for (int t = 0; t < num_threads; t++) {
            td[t].start_row = start_row;
            td[t].end_row = start_row + block_size;

            if (t == num_threads - 1) {
                td[t].end_row += remaining_blocks;
            }
            td[t].start_col = 0;
            td[t].end_col = m;
            td[t].md = &md;

            if (pthread_create(&threads[t], NULL, mult_block, (void*)&td[t]) != 0) {
                fprintf(stderr, "Erro ao criar thread %d\n", t);
                exit(EXIT_FAILURE);
            }

            start_row = td[t].end_row;
        }

        // Aguarda o término das threads
        for (int t = 0; t < num_threads; t++) {
            if (pthread_join(threads[t], NULL) != 0) {
                fprintf(stderr, "Erro no join da thread %d\n", t);
            }
        }

        printf("Matriz A:\n");
        for (int i = 0; i < md.n1; i++) {
            for (int j = 0; j < md.m1; j++) {
                printf("%.2lf ", md.matriz1[i][j]);
            }
            printf("\n");
        }

        printf("Matriz B:\n");
        for (int i = 0; i < md.n2; i++) {
            for (int j = 0; j < md.m2; j++) {
                printf("%.2lf ", md.matriz2[i][j]);
            }
            printf("\n");
        }

        printf("Matriz Resultante:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                printf("%.2lf ", md.result[i][j]);
            }
            printf("\n");
        }

        // Liberação de memória
        free(td);
        free(threads);
        for (int i = 0; i < n; i++) {
            free(md.matriz1[i]);
        }
        free(md.matriz1);
        for (int i = 0; i < md.n2; i++) {
            free(md.matriz2[i]);
        }
        free(md.matriz2);

        for (int i = 0; i < n; i++) {
            free(md.result[i]);
        }
        free(md.result);
    }

    fclose(input);
    return 0;
}
