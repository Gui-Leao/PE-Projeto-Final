#include <stdio.h>
#include <stdlib.h>
#include "auxiliar.h"
#include "bignumber.h"


/* 
* @brief Cria um Nó.
*
* @param digit Dígito do Nó.
* @return O Nó criado.
*
* @details A função inicializa o Nó apenas com o dígito, sem nenhuma ligação com algum Nó
*          posterior ou anterior.
*/

Node* create_node(int digit) {
    Node* new_node = (Node*)malloc(sizeof(Node));

    new_node->digit = digit;
    new_node->next_digit = NULL;
    new_node->prev_digit = NULL;

    return new_node;
}


/* 
* @brief Cria um Big Number.
*
* @param str_number String do número.
* @return O Big Number criado.
*
* @details A função adiciona cada Nó a medida que lê cada dígito da string, ou seja, a cada
*          iteração, um novo Nó é adicionado ao final do Big Number. Cada dígito lido da
*          string é convertido em inteiro, antes de ser usado para a criação de um Nó.
*/

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


/* 
* @brief Realiza o print de um Big Number.
*
* @param big_number Big Number a ser printado.
*/

void print_big_number(BigNumber *big_number) {
    if ((big_number->is_positive == 0)) printf("-");

    Node* current_node = big_number->first_digit;

    while (current_node != NULL) {
        printf("%d", current_node->digit);
        current_node = current_node->next_digit;
    }

    printf("\n");
}


/* 
* @brief Libera a memória alocada pelo Big Number.
*
* @param big_number Big Number a ser liberado da memória.
*/

void free_big_number(BigNumber *big_number) {
    Node* current_node = big_number->first_digit;   

    while (current_node != NULL) {
        Node* next_node = current_node->next_digit;
        free(current_node);
        current_node = next_node;
    }

    free(big_number);
}


/* 
* @brief Realiza a soma entre dois Big Numbers.
*
* @param x Big Number a ser somado.
* @param y Big Number a ser somado.
*
* @details A função faz, primeiramente, a validação de sinais da operação. Se observado
*          sinais diferentes entre os dois números, a função de subtração é acionada.
*          Caso a soma seja de fato realizada, é iniciado a partir do último Nó dos 
*          Big Numbers, e só finaliza quando não há mais nenhum Nó, tanto do Big Number x, 
*          ou do Big Number y, além de não precisar somar o dígito de transporte.
*
* @return BigNumber result Resultado da operação.
*/

BigNumber* sum_big_numbers(BigNumber *x, BigNumber *y) {
    BigNumber* result = create_big_number("");

    if (x->is_positive != y->is_positive) {
        int comparison_big_numbers_modules = compare_big_numbers_modules(x, y);
        
        if (comparison_big_numbers_modules == 1) {
            result = switch_to_sum_or_subtraction("sub", x->is_positive, x, y, result);
        }
        
        else if (comparison_big_numbers_modules == -1) {
            result = switch_to_sum_or_subtraction("sub", y->is_positive, y, x, result);  
        } 
        
        else {
            add_node_to_big_number(result, 0);
            return result;
        }
    }
    
    else {
        if (x->is_positive == 0 && y->is_positive == 0) {
            result->is_positive = 0;
        } else {
            result->is_positive = 1;
        }

        Node* node_x = x->last_digit;
        Node* node_y = y->last_digit;

        int carry_digit = 0;

        while (node_x != NULL || node_y != NULL || carry_digit > 0) {
            int digit_x, digit_y, sum, new_result_digit;

            digit_x = (node_x != NULL) ? node_x->digit : 0;
            digit_y = (node_y != NULL) ? node_y->digit : 0;

            sum = digit_x + digit_y + carry_digit;
            carry_digit = sum / 10;
            new_result_digit = sum % 10;

            add_node_to_big_number(result, new_result_digit);
            
            if (node_x != NULL) node_x = node_x->prev_digit;
            if (node_y != NULL) node_y = node_y->prev_digit;
        }
    }

    return result;
}


/* 
* @brief Realiza a subtração entre dois Big Numbers.
*
* @param x Big Number a ser subtraído.
* @param y Big Number a ser subtraído.
*
* @details Em primeiro momento, verificamos se os dois números tem sinais diferentes,
*          para que, dessa forma, a função de soma seja acionada. O resultado dessa soma
*          será determinado olhando para x, já que, se possuem sinais diferentes, a subtração
*          irá inverter o sinal de y, logo, quando x é positivo, temos y negativo, resultando
*          em uma subtração do tipo x - (-y) = x + y (uma soma com sinal positivo). 
*          Quando x é negativo, temos y positivo, resultando em uma subtração do tipo 
*          -x - (y) = -x - y (uma soma com sinal negativo).
*
*          Quando a subtração é de fato acionada (dois números com mesmo sinal).
*          O sinal do resultado é determinado a partir dos valores da operação, além de
*          rearranjar a ordem da subtração, colocando o Big Number de maior valor em primeiro,
*          se necessário. A operação de subtração é feita dígito a dígito, verificando a necessidade
*          de empréstimo quando necessário, além de subtrair esse empréstimo na próxima iteração.       
*
* @return BigNumber result Resultado da operação.
*/

BigNumber* subtraction_big_numbers(BigNumber *x, BigNumber *y) {
    BigNumber* result = create_big_number("");

    if (x->is_positive != y->is_positive) {
        result = switch_to_sum_or_subtraction("sum", x->is_positive, x, y, result);
    }

    else {
        Node* node_x = x->last_digit;
        Node* node_y = y->last_digit;

        int result_sign = determine_sign_in_subtraction(x, y);
        determine_order_of_subtraction(x, &node_x, y, &node_y, result);

        int borrow_digit = 0;

        while (node_x != NULL || node_y != NULL) {
            int digit_x, digit_y, subtraction;

            digit_x = (node_x != NULL) ? node_x->digit : 0;
            digit_y = (node_y != NULL) ? node_y->digit : 0;
        
            subtraction = digit_x - digit_y - borrow_digit;

            if (subtraction < 0) {
                subtraction += 10;
                borrow_digit = 1;
            } else {
                borrow_digit = 0;
            }

            add_node_to_big_number(result, subtraction);

            if (node_x != NULL) node_x = node_x->prev_digit;
            if (node_y != NULL) node_y = node_y->prev_digit;
        }

        result->is_positive = result_sign;
    }

    remove_zeros_from_left(result);

    return result;
}