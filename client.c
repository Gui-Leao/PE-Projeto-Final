#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bignumber.h"
#include "auxiliar.h"

int main() {
    while (1) {
        char* number_1 = read_input();
        
        if(strlen(number_1) == 0) {
            free(number_1);
            break;
        }

        char* number_2 = read_input();
        char* operation = read_input();

        BigNumber* big_num1 = create_big_number(number_1);
        BigNumber* big_num2 = create_big_number(number_2);
        BigNumber* result = NULL;

        switch (*operation) {
            case '+':
                result = sum_big_numbers(big_num1, big_num2);
                break;
            case '-':
                result = subtraction_big_numbers(big_num1, big_num2);
                break;
        }

        print_big_number(result);

        free_big_number(big_num1);
        free_big_number(big_num2);
        free_big_number(result);
        free(number_1);
        free(number_2);
        free(operation);
    }

    return 0;
}