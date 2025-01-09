#ifndef bignumber_h
#define bignumber_h

typedef struct Node {
    char digit;
    struct Node *next_digit;
    struct Node *prev_digit;
} Node;

typedef struct BigNumber {
    int is_positive;
    Node *first_digit;
    Node *last_digit;
} BigNumber;

BigNumber* create_big_number(char *str_number);
BigNumber* sum_big_numbers(BigNumber *x, BigNumber *y);
BigNumber* subtract_big_numbers(BigNumber *x, BigNumber *y);
BigNumber* divide_big_numbers(BigNumber *x, BigNumber *y);
BigNumber* multiply_big_numbers(BigNumber *x, BigNumber *y);

void print_big_number(BigNumber *x);
void free_big_number(BigNumber *x);

#endif