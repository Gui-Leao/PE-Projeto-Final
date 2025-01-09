#include <stdio.h>
#include <stdlib.h>
#include "bignumber.h"

Node* create_node(int digit) {
    Node* new_node = (Node*)malloc(sizeof(Node));

    new_node->digit = digit;
    new_node->next_digit = NULL;
    new_node->prev_digit = NULL;

    return new_node;
};

BigNumber* create_big_number(char *str_number) {
    BigNumber* big_number = (BigNumber*)malloc(sizeof(BigNumber));

    big_number->first_digit = NULL;
    big_number->last_digit = NULL;
    big_number->is_positive = 1;

    int i = 0;

    if (str_number[i] == '-') {
        big_number->is_positive = 0;
        i++;
    }

    while (str_number[i] != '\0') {
        int digit = str_number[i] - '0';
        Node* new_node = create_node(digit);

        if (big_number->first_digit == NULL) {
            big_number->first_digit = new_node;
            big_number->last_digit = new_node;
        } else {
            big_number->last_digit->next_digit = new_node;
            new_node->prev_digit = big_number->last_digit;
            big_number->last_digit = new_node;
        }

        i++;
    }

    return big_number;
}