#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int n1, m1, n2, m2;
    double** matriz1;
    double** matriz2;
} MatrixData;

typedef struct {
    int c, n, m;
    double** result;
    double** matriz1;
    double** matriz2;
} ThreadData;

void* multmx(void* args) {
    ThreadData* td = (ThreadData*)args;
    int c = td->c;
    int n = td->n;
    int m = td->m;
    double** result = td->result;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            result[i][j] = 0;
            for (int k = 0; k < n; k++) {
                result[i][j] += td->matriz1[i][k] * td->matriz2[k][j];
            }
        }
    }

    printf("M%d:\n", c);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%.2lf ", result[i][j]);
        }
        printf("\n");
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    printf("Programa comecou!\n");
    printf("Arquivo de entrada = %s\n", argv[1]);
    FILE* input = fopen(argv[1], "r");
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

        ThreadData* td = (ThreadData*)malloc(sizeof(ThreadData));
        td->c = c;
        td->n = n;
        td->m = m;
        td->result = result;
        td->matriz1 = md[c].matriz1;
        td->matriz2 = md[c].matriz2;

        if (pthread_create(&threads[c], NULL, multmx, (void*)td) != 0) {
            fprintf(stderr, "Erro ao criar thread %d\n", c);
            exit(EXIT_FAILURE);
        }
    }

    for (int c = 0; c < num_iter; c++) {
        if (pthread_join(threads[c], NULL) != 0) {
            fprintf(stderr, "Erro no join da thread %d\n", c);
        }
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
