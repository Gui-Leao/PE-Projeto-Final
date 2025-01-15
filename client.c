#include <stdio.h>
#include <stdlib.h>
#include "bignumber.h"

int main() {
    BigNumber* x = create_big_number("7");
    BigNumber* y = create_big_number("-100");

    BigNumber* w = create_big_number("-10");
    BigNumber* z = create_big_number("3");


    BigNumber* sum = sum_big_numbers(x, y);
    BigNumber* subtraction = subtraction_big_numbers(w, z);

    printf("Soma: ");
    print_big_number(sum);
    printf("\n");

    printf("Subtração: ");
    print_big_number(subtraction);
    printf("\n");

    free_big_number(x);
    free_big_number(y);
    free_big_number(sum);
    free_big_number(subtraction);

    return 0;
}