#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int n1, m1, n2, m2;
    double** matriz1;
    double** matriz2;
} MatrixData;

typedef struct {
    int c, n, m, start_row, end_row;
    double** result;
    double** matriz1;
    double** matriz2;
} ThreadData;

void* multmx(void* args) {
    ThreadData* td = (ThreadData*)args;
    int c = td->c;
    int n = td->n;
    int m = td->m;
    int start_row = td->start_row;
    int end_row = td->end_row;
    double** result = td->result;

    // Realiza a multiplicação das matrizes para a parte atribuída à thread
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < m; j++) {
            result[i][j] = 0;
            for (int k = 0; k < n; k++) {
                result[i][j] += td->matriz1[i][k] * td->matriz2[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    printf("Programa comecou!\n");
    FILE* input = fopen(argv[1], "r");
    if (access(argv[1], R_OK) != 0) {
        printf("Arquivo nao possui permissao de leitura\n");
    }

    if (input == NULL) {
        printf("FOPEN gerou ponteiro nulo!\n");
        return 1;
    }
    printf("Arquivo de entrada = %s\n", argv[1]);
    int num_iter;
    fscanf(input, "%d", &num_iter);
    printf("num_iter = %d\n", num_iter);

    pthread_t* threads = (pthread_t*)malloc(num_iter * sizeof(pthread_t));
    MatrixData* md = (MatrixData*)malloc(num_iter * sizeof(MatrixData));

    for (int c = 0; c < num_iter; c++) {
        fscanf(input, "%d %d", &md[c].n1, &md[c].m1);
        printf("%d %d\n", md[c].n1, md[c].m1);
        md[c].matriz1 = (double**)malloc(md[c].n1 * sizeof(double*));
        for (int i = 0; i < md[c].n1; i++) {
            md[c].matriz1[i] = (double*)malloc(md[c].m1 * sizeof(double));
            for (int j = 0; j < md[c].m1; j++) {
                fscanf(input, "%lf", &md[c].matriz1[i][j]);
            }
        }

        fscanf(input, "%d %d", &md[c].n2, &md[c].m2);
        printf("%d %d\n", md[c].n2, md[c].m2);
        md[c].matriz2 = (double**)malloc(md[c].n2 * sizeof(double*));
        for (int i = 0; i < md[c].n2; i++) {
            md[c].matriz2[i] = (double*)malloc(md[c].m2 * sizeof(double));
            for (int j = 0; j < md[c].m2; j++) {
                fscanf(input, "%lf", &md[c].matriz2[i][j]);
            }
        }

        int n = md[c].n1;
        int m = md[c].m2;
        double** result = (double**)malloc(n * sizeof(double*));
        for (int i = 0; i < n; i++) {
            result[i] = (double*)malloc(m * sizeof(double));
        }

        int num_threads = sysconf(_SC_NPROCESSORS_ONLN);; // Número de threads para paralelizar uma instância de multiplicação
        int rows_per_thread = n / num_threads;
        int remaining_rows = n % num_threads;

        ThreadData* td = (ThreadData*)malloc(num_threads * sizeof(ThreadData));
        for (int t = 0; t < num_threads; t++) {
            td[t].c = c;
            td[t].n = n;
            td[t].m = m;
            td[t].start_row = t * rows_per_thread;
            td[t].end_row = (t + 1) * rows_per_thread;
            if (t == num_threads - 1) {
                td[t].end_row += remaining_rows;
            }
            td[t].result = result;
            td[t].matriz1 = md[c].matriz1;
            td[t].matriz2 = md[c].matriz2;

            if (pthread_create(&threads[c], NULL, multmx, (void*)&td[t]) != 0) {
                fprintf(stderr, "Erro ao criar thread %d\n", t);
                exit(EXIT_FAILURE);
            }
        }

        for (int t = 0; t < num_threads; t++) {
            if (pthread_join(threads[c], NULL) != 0) {
                fprintf(stderr, "Erro no join da thread %d\n", t);
            }
        }

        free(td);
        for (int i = 0; i < n; i++) {
            free(result[i]);
        }
        free(result);
    }

    fclose(input);
    free(threads);
    for (int c = 0; c < num_iter; c++) {
        for (int i = 0; i < md[c].n1; i++) {
            free(md[c].matriz1[i]);
        }
        free(md[c].matriz1);
        for (int i = 0; i < md[c].n2; i++) {
            free(md[c].matriz2[i]);
        }
        free(md[c].matriz2);
    }
    free(md);
    return 0;
}
