#include <stdio.h>
#include <stdlib.h>
#include "bignumber.h"

int main() {
    char *str_number = "-2012";
    BigNumber* big_number = create_big_number(str_number);

    print_big_number(big_number);

    printf("\n");

    printf("Soma de dois Big Numbers:\n");

    BigNumber* x = create_big_number("111");
    BigNumber* y = create_big_number("10");
    BigNumber* sum = sum_big_numbers(x, y);

    print_big_number(sum);
    printf("\n");

    return 0;
}