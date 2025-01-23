#ifndef bignumber_h
#define bignumber_h

#include <stdbool.h>

typedef struct Node {
    char digit;
    struct Node *next_digit;
    struct Node *prev_digit;
}* Node;

typedef struct BigNumber {
    bool is_positive;
    bool is_even;
    int num_digits;
    Node first_digit;
    Node last_digit;
}* BigNumber;

Node create_node(int digit);

BigNumber create_big_number(char *str_number);

void print_big_number(BigNumber x);
void free_big_number(BigNumber x);

BigNumber sum_big_numbers(BigNumber x, BigNumber y);
BigNumber subtract_big_numbers(BigNumber x, BigNumber y);
BigNumber divide_big_numbers(BigNumber dividend, BigNumber divisor);
BigNumber multiply_big_numbers(BigNumber x, BigNumber y);
BigNumber fast_exponentiation(BigNumber base, BigNumber exponent);
BigNumber remainder_of_division(BigNumber dividend, BigNumber divisor);
BigNumber divide_by_power_of_ten(BigNumber x, int power);
BigNumber get_remainder_by_power_of_ten(BigNumber x, int power);
BigNumber multiply_karatsuba_big_numbers(BigNumber x, BigNumber y);

#endif
