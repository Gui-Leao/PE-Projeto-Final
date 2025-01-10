#include <stdio.h>
#include <stdlib.h>
#include "bignumber.h"

int main() {
    // Teste criando um número negativo (na mão):
    char *str_number = "-2012";
    BigNumber* big_number = create_big_number(str_number);

    print_big_number(big_number);
    free_big_number(big_number);

    printf("\n");

    // Teste criando uma soma:
    BigNumber* x = create_big_number("111");
    BigNumber* y = create_big_number("10");
    BigNumber* sum = sum_big_numbers(x, y);

    print_big_number(sum);

    free_big_number(x);
    free_big_number(y);
    free_big_number(sum);

    printf("\n");

    return 0;
}