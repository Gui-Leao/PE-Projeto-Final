#include <stdio.h>
#include "bignumber.h"

int main() {
    char *str_number = "-6531932423423432";
    BigNumber* big_number = create_big_number(str_number);

    Node* current_node = big_number->first_digit;

    while (current_node != NULL) {
        printf("%d", current_node->digit);
        current_node = current_node->next_digit;
    }

    printf("\n");

    return 0;
}