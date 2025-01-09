#include <stdio.h>
#include <stdlib.h>
#include "bignumber.h"

int main() {
    char *str_number = "1";
    BigNumber* big_number = create_big_number(str_number);

    Node* current_node = big_number->first_digit;

    while (current_node != NULL) {
        printf("%d", current_node->digit);
        current_node = current_node->next_digit;
    }

    printf("\n");

    printf("Soma de dois BigNumbers:\n");

    BigNumber* x = create_big_number("111");
    BigNumber* y = create_big_number("10");
    BigNumber* sum = sum_big_numbers(x, y);

    current_node = sum->first_digit;

    while (current_node != NULL) {
        printf("%d", current_node->digit);
        current_node = current_node->next_digit;
    }

    printf("\n");

    return 0;
}