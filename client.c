#include <stdio.h>
#include <stdlib.h>
#include "bignumber.h"

int main() {
    BigNumber* x = create_big_number("-111");
    BigNumber* y = create_big_number("11");

    int comparison_big_numbers = return_largest_big_number(x, y);
    printf("A comparação entre os Big Numbers retornou: %d\n", comparison_big_numbers);

    BigNumber* sum = sum_big_numbers(x, y);

    print_big_number(sum);

    free_big_number(x);
    free_big_number(y);
    free_big_number(sum);

    printf("\n");

    return 0;
}