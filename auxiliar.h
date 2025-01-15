#ifndef auxiliar_h
#define auxiliar_h

#include "bignumber.h"

void add_node_to_big_number(BigNumber *big_number, int digit);

int get_big_number_length(BigNumber *big_number);
int compare_big_numbers_modules(BigNumber *x, BigNumber *y);

BigNumber* switch_to_sum_or_subtraction(char *switch_to, int sign, BigNumber *x, BigNumber *y, BigNumber *result);

int determine_sign_in_subtraction(BigNumber *x, BigNumber *y);
void determine_order_of_subtraction(BigNumber* x, Node** node_x, BigNumber* y, Node** node_y, BigNumber* result);
void remove_zeros_from_left(BigNumber *big_number);


#endif