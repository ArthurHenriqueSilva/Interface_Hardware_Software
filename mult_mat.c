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
	for(int c = 0; c < num_iter*2; c++){
		int n, m;
		fscanf(input, "%d %d", &n, &m);
		printf("%d %d\n", n, m);
		double **matriz = (double **)malloc(n * sizeof(double *));
		for(int i = 0; i < n; i++ ){
			matriz[i] = (double *)malloc(m * sizeof(double));
			for(int j = 0; j < m; j++){
				fscanf(input, "%lf", &matriz[i][j]);
			}
		}
		
		printf("M%d:\n", c);
		for(int i = 0; i < n; i++){
			for(int j = 0; j < m; j++){
				printf("%.2lf ", matriz[i][j]);
			}
			printf("\n");
		}
		
		for(int i = 0; i < n; i++){
			free(matriz[i]);
		}
	}
	fclose(input);
	return 0;
}
