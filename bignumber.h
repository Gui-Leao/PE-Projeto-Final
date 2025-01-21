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

BigNumber create_big_number(char *str_number);
BigNumber sum_big_numbers(BigNumber x, BigNumber y);
BigNumber subtraction_big_numbers(BigNumber x, BigNumber y);
BigNumber multiply_big_numbers(BigNumber x, BigNumber y);
BigNumber divide_big_numbers(BigNumber dividend, BigNumber divisor);
BigNumber remainder_of_division(BigNumber dividend, BigNumber divisor);
BigNumber fast_exponentiation(BigNumber base, BigNumber exponent);
BigNumber fast_exponentiation_norecursion(BigNumber dividend, BigNumber divisor);
BigNumber multiply_karatsuba_big_numbers(BigNumber x, BigNumber y);
void print_big_number(BigNumber x);
void free_big_number(BigNumber x);

#endif