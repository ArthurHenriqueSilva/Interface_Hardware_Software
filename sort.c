#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]){
  FILE* input = fopen(argv[1], "r");
  FILE* output = fopen(argv[2], "w");

  int num_loop_externo;
  fscanf(input, "%d", &num_loop_externo);
  printf("%i\n", num_loop_externo);
  return 0;
}
