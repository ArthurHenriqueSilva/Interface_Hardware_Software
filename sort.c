#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

void bubbleSort(int v[], int s){
  int i, j, temp;
  for(int i = 0; i < s - 1; i++){
    for(int j = 0; j < s - 1 - i; j++){
      if(v[j] > v[j+1]){
        temp = v[j];
        v[j] = v[j+1];
        v[j+1] = temp;
      }
    }
  }
}
int main(int argc, char* argv[]){
  FILE* input = fopen(argv[1], "r");
  FILE* output = fopen(argv[2], "w");

  int num_outer_loop;
  fscanf(input, "%d", &num_outer_loop);
  for(int i1 = 0; i1 < num_outer_loop; i1++){
    int vector_size;
    fscanf(input, "%d", &vector_size);
    int* vector = (int* )malloc(vector_size * sizeof(int));
    for(int i = 0; i < vector_size; i ++){
      fscanf(input, "%d", &vector[i]);
    }
    bubbleSort(vector, vector_size);
    fprintf(output, "[%d] ", i1 );
    for(int i = 0; i < vector_size; i++){
      fprintf(output, "%d ", vector[i]);
    }
    fprintf(output, "\n");
    
  }
  return 0;
}
