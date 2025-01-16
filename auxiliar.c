#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliar.h"
#include "bignumber.h"

char* read_input() {
    int capacity = 16;
    int size = 0;
    char* input = malloc(capacity);

    int c;

    while ((c = getchar()) != EOF && c != '\n') {
        input[size++] = c;

        if (size + 1 >= capacity) {
            capacity *= 2;

            char* new_input = realloc(input, capacity);
            input = new_input;
        }
    }

    input[size] = '\0';

    return input;
}

/* 
* @brief Adiciona um novo Nó em um Big Number.
*
* @param big_number Registro do tipo Big Number.
* @param digit Número que representa o Nó a ser criado.
*
* @details A função atualiza os campos do Nó criado (dígito posterior e anterior),
*          além alocar o novo Nó na primeira posição do Big Number.
*/

void add_node_to_big_number(BigNumber *big_number, int digit) {
    Node* new_node = (Node*)malloc(sizeof(Node));

    new_node->digit = digit;
    new_node->next_digit = big_number->first_digit;
    new_node->prev_digit = NULL;

    if (big_number->first_digit == NULL) big_number->last_digit = new_node;
    if (big_number->first_digit != NULL) big_number->first_digit->prev_digit = new_node;
        
    big_number->first_digit = new_node;
}


/* 
* @brief Soma a quantidade de dígitos de um Big Number.
*
* @param x Big Number a ter os dígitos contados.
*
* @details A função passa por todos os Nós do Big Number e soma a quantidade.
*
* @return int length Tamanho do Big Number
*/

int get_big_number_length(BigNumber *big_number) {
    int length = 0;

    Node *current_node = big_number->first_digit;

    while (current_node) {
        length++;
        current_node = current_node->next_digit;
    }

    return length;
}


/* 
* @brief Compara o módulo de dois Big Numbers.
*
* @param x Big Number a ser comparado.
* @param y Big Number a ser comparado.
*
* @details A função primeiro compara o tamanho de cada Big Number, se
*          não for possível constatar o maior valor por meio dessa forma,
*          a função avança comparando dígito a dígito de cada Big Number.
*
* @return int 1,  se x > y
* @return int -1, se x < y
* @return int 0,  se x = y
*/

int compare_big_numbers_modules(BigNumber *x, BigNumber *y) {
    int len_x = get_big_number_length(x);
    int len_y = get_big_number_length(y);

    if (len_x > len_y) return 1;
    if (len_x < len_y) return -1;

    Node *node_x = x->first_digit;
    Node *node_y = y->first_digit;

    while (node_x && node_y) {
        if (node_x->digit > node_y->digit) return 1;
        if (node_x->digit < node_y->digit) return -1;

        node_x = node_x->next_digit;
        node_y = node_y->next_digit;
    } 

    return 0;
}


/* 
* @brief A função é responsável por executar uma operação inversa (entre soma ou subtração)
*        quando acionada.
*
* @param char switch_to Define para qual operação vai ser trocada (sum ou sub).
* @param int sign Sinal do número que vai servir como base para o sinal do resultado final.
* @param x Big Number a ser somado ou subtraído.
* @param y Big Number a ser somado ou subtraído.
* @param result Big Number que carrega o resultado final.
*
* @details A função começa verificando o sinal do resultado final, sempre a partir do
*          primeiro número da operação entre sinais diferentes, já que ele não vai
*          ter o sinal trocado. Logo após, os atributos "is_positive" é setado como
*          positivo, para que nas operações a serem acionadas, não ter nenhuma manipulação
*          de sinais, já que já temos o sinal do resultado.
*
* @return BigNumber result Resultado da operação.
*/

BigNumber* switch_to_sum_or_subtraction(char *switch_to, int sign, BigNumber *x, BigNumber *y, BigNumber *result) {
    int result_sign = 1 ? sign == 1 : 0;

    x->is_positive = 1;
    y->is_positive = 1;
    
    if (strcmp(switch_to, "sub") == 0) {
        result = subtraction_big_numbers(x, y);
        result->is_positive = result_sign;
    }

    else {
        result = sum_big_numbers(x, y);
        result->is_positive = result_sign;
    }

    return result;
}


/* 
* @brief Determina o sinal do resultado de uma subtração quando os dois Big Numbers
*        possuem o mesmo sinal.
*
* @param x Big Number.
* @param y Big Number.
*
* @details Se os dois números forem positivos na subtração, teremos uma subtração do tipo
*          x - y, logo, o resultado será positivo se x > y. Para o caso contrário, 
*          em que os dois são negativos, teremos uma subtração do tipo -x + y, logo, 
*          o resultado será positivo se y > x.
*
* @return 1, para resultado positivo.
* @return 0, para resultado negativo.
* @return -1, para caso inválido.
*/

int determine_sign_in_subtraction(BigNumber *x, BigNumber *y) {
    int comparison_big_numbers_modules = compare_big_numbers_modules(x, y);

    if (x->is_positive == 0) {
        return 1 ? comparison_big_numbers_modules == -1 : 0;
    }

    else if (x->is_positive == 1) {
        return 1 ? comparison_big_numbers_modules == 1 : 0;
    }

    return -1;
}


/* 
* @brief Determina a ordem dos números em uma subtração.
*
* @param x Big Number.
* @param y Big Number.
*
* @details A função de subtração, para funcionar corretamente, deve ter o maior valor na
*          na primeira ordem da subtração. Se observado que y > x, os Nós são invertidos.
*          A função também verifica se os dois números são exatamente iguais, retornando 0,
*          nesse caso.
* 
*/

void determine_order_of_subtraction(BigNumber* x, Node** node_x, BigNumber* y, Node** node_y, BigNumber* result) {
    int comparison_big_numbers_modules = compare_big_numbers_modules(x, y);   

    if (comparison_big_numbers_modules == -1) {
        *node_x = y->last_digit;
        *node_y = x->last_digit;
    }

    else if (comparison_big_numbers_modules == 0) {
        add_node_to_big_number(result, 0);
        return;
    }
}


/* 
* @brief Remove zeros da esquerda que permanecem no resultado depois de alguma operação
*        com os Big Numbers.
*
* @param big_number Big Number a ter os zeros removidos da esquerda.
*
* @details A função passa por por todos os dígitos da esquerda que são iguais a zero,
*          removendo o Nó do Big Number e alterando qual será o novo primeiro Nó.
*          Se o Big Number for exatamente igual a zero, nada é feito.
*/

void remove_zeros_from_left(BigNumber *big_number) {
    while (big_number->first_digit->digit == 0 && big_number->first_digit != big_number->last_digit) {
        Node *node_to_remove = big_number->first_digit;

        big_number->first_digit = node_to_remove->next_digit;
        big_number->first_digit->prev_digit = NULL;

        free(node_to_remove);
    }

    if (big_number->first_digit == big_number->last_digit && big_number->first_digit->digit == 0) {
        big_number->is_positive = 1;
    }
}