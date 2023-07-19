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
    int c, start_row, end_row, start_col, end_col;
    double** result;
    double** matriz1;
    double** matriz2;
} ThreadData;

void* multmx(void* args) {
    ThreadData* td = (ThreadData*)args;
    int r;
    // Realiza a multiplicação das matrizes para a parte atribuída à thread
    for (int i = td->start_row; i < td->end_row; i++) {
        for (int j = td->start_col; j < td->end_col; j++) {
            r = 0;
            for (int k = td->start_row; k < td->end_row; k++) {
                r += td->matriz1[i][k] * td->matriz2[k][j];
            }
            td->result[i][j] = r;
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    printf("Programa comecou!\n");
    FILE* input = fopen(argv[1], "r");
    if (access(argv[1], R_OK) != 0) {
        printf("Arquivo nao possui permissao de leitura.\n");
        return 1;
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

        int num_threads = sysconf(_SC_NPROCESSORS_ONLN);
        printf("num_threads %d\n", num_threads);
        int rows_per_thread = n / num_threads;
        int cols_per_thread = m / num_threads;
        int remaining_rows = n % num_threads;
        int remaining_cols = m % num_threads;

        ThreadData* td = (ThreadData*)malloc(num_threads * sizeof(ThreadData));
        for (int t = 0; t < num_threads; t++) {
            td[t].c = c;
            td[t].start_row = t * rows_per_thread;
            td[t].end_row = (t + 1) * rows_per_thread;
            td[t].start_col = t * cols_per_thread;
            td[t].end_col = (t + 1) * cols_per_thread;

            if (t == num_threads - 1) {
                td[t].end_row += remaining_rows;
                td[t].end_col += remaining_cols;
            }
            td[t].result = result;
            td[t].matriz1 = md[c].matriz1;
            td[t].matriz2 = md[c].matriz2;

            if (pthread_create(&threads[t], NULL, multmx, (void*)&td[t]) != 0) {
                fprintf(stderr, "Erro ao criar thread %d\n", t);
                exit(EXIT_FAILURE);
            }
        }

        printf("Matriz A:\n");
        for (int i = 0; i < md[c].n1; i++) {
            for (int j = 0; j < md[c].m1; j++) {
                printf("%.2lf ", md[c].matriz1[i][j]);
            }
            printf("\n");
        }

        printf("Matriz B:\n");
        for (int i = 0; i < md[c].n2; i++) {
            for (int j = 0; j < md[c].m2; j++) {
                printf("%.2lf ", md[c].matriz2[i][j]);
            }
            printf("\n");
        }

        for (int t = 0; t < num_threads; t++) {
            if (pthread_join(threads[t], NULL) != 0) {
                fprintf(stderr, "Erro no join da thread %d\n", t);
            }
        }

        printf("Matriz Resultante:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                printf("%.2lf ", result[i][j]);
            }
            printf("\n");
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
