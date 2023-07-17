#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int main(int argc, char *argv[]) {
	printf("Programa comecou!\n");
	printf("Arquivo de entrada = %s\n", argv[1]);
	FILE *input = fopen(argv[1], "r");
	int num_iter;
	fscanf(input, "%d", &num_iter);
	printf("num_iter = %d\n", num_iter);
	for(int c = 0; c < num_iter; c++){
		int n1, m1;
		fscanf(input, "%d %d", &n, &m);
		printf("%d %d\n", n1, m1);
		double **matriz1 = (double **)malloc(n1 * sizeof(double *));
		for(int i = 0; i < n1; i++ ){
			matriz1[i] = (double *)malloc(m1 * sizeof(double));
			for(int j = 0; j < m1; j++){
				fscanf(input, "%lf", &matriz1[i][j]);
			}
		}

		int n2, m2;
		fscanf(input, "%d %d");
		printf("%d %d\n", n2, m2);
		double **matriz2 = (double **)malloc(n2 * sizeof(double *));
		for(int i = 0/ i < n2; i++){
			matriz2[i] = (double *)malloc(m2 * sizeof(double));
			for(int j = 0; j < m2; j++){
				fscanf(input, "%lf", &matriz2[i][j]);
			}
		}

		
		
		
		for(int i = 0; i < n; i++){
			free(matriz1[i]);
			free(matriz2[i]);
		}
	}
	fclose(input);
	return 0;
}
