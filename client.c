#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliar.h"
#include <time.h>

int main() {
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    execute_program();
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Tempo de execucao: %f segundos\n", cpu_time_used);
    printf("Fim do programa!\n");
    return 0;
}