#include <stdio.h>
#include <stdlib.h>
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
* @brief Adiciona um novo Nó em um Big Number.
*
* @param big_number Registro do tipo Big Number.
* @param digit Número que representa o nó a ser criado.
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
            int result_sign = 1 ? x->is_positive == 1 : 0;

            x->is_positive = 1;
            y->is_positive = 1;

            result = subtraction_big_numbers(x, y);
            result->is_positive = result_sign;
        } 
        
        else if (comparison_big_numbers_modules == -1) {
            int result_sign = 1 ? y->is_positive == 1 : 0;

            x->is_positive = 1;
            y->is_positive = 1;

            result = subtraction_big_numbers(y, x);
            result->is_positive = result_sign;
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
        int result_sign = 1 ? x->is_positive == 1 : 0;

        x->is_positive = 1;
        y->is_positive = 1;

        result = sum_big_numbers(x, y);
        result->is_positive = result_sign;
    }

    else {
        int result_sign = determine_sign_in_subtraction(x, y);

        Node* node_x = x->last_digit;
        Node* node_y = y->last_digit;
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