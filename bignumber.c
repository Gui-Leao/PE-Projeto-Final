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
* @brief Verifica os tamanhos entre dois Big Numbers.
*
* @param x Big Number a ser comparado.
* @param y Big Number a ser comparado.
*
* @details Função auxiliar para verificar, dígito a dígito (Nó) de cada Big Number 
*          e retornar qual possui o maior tamanho, logo, o maior valor. 
*
* @return 1, para x > y.
* @return -1, para x < y.
* @return 0, para x e y com mesmo tamanho.
*/

int compare_big_numbers_length(BigNumber *x, BigNumber *y) {
    Node *node_x = x->first_digit;
    Node *node_y = y->first_digit;

    int len_x = 0, len_y = 0;

    while (node_x) {
        len_x++;
        node_x = node_x->next_digit;
    }

    while (node_y) {
        len_y++;
        node_y = node_y->next_digit;
    }

    if (len_x > len_y) return 1;
    if (len_x < len_y) return -1;

    return 0;
}


/* 
* @brief Função auxiliar para verificar, daqueles Big Numbers que possuem o mesmo tamanho, 
*        qual é o maior. A função compara apenas o módulo dos números, sem considerar o sinal.
*
* @param x Big Number a ser comparado.
* @param y Big Number a ser comparado.
*
* @details A função parte do primeiro Nó (dígito) de cada Big Number, verificando, entre eles,
*          qual é o maior. Se os dígitos forem iguaís, partimos para o próximo Nó, realizando
*          a verificação novamente. Isso se repete, enquanto os dígitos forem iguais, até não
*          ter mais dígitos para serem comparados, constatando que os números são iguais.
*
* @return 1, para x > y.
* @return -1, para x < y.
* @return 0, para x e y iguais.
*/

int compare_big_numbers_with_same_length(BigNumber *x, BigNumber *y) {
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
* @brief Agrega as funções de comparação para retornar o maior Big Number.
*
* @param x Big Number a ser comparado.
* @param y Big Number a ser comparado.
*
* @details É usado a função de comparação do tamanho (compare_big_numbers_length),
*          junto com a comparação de valor para os que possuem o mesmo tamanho
*          (compare_big_numbers_with_same_length), para que, no fim, retorne de fato
*          qual é o Big Number de maior valor, ou se possuem o mesmo valor.
*
* @return 1, para x > y.
* @return -1, para x < y.
* @return 0, para x e y iguais.
*/

int return_largest_big_number(BigNumber *x, BigNumber *y) {
    int length_comparison = compare_big_numbers_length(x, y);
    if (length_comparison != 0) return length_comparison;

    return compare_big_numbers_with_same_length(x, y);
}


/* 
* @brief Realiza a soma entre dois Big Numbers.
*
* @param x Big Number a ser somado.
* @param y Big Number a ser somado.
*
* @details A função inicializa a soma a partir do último Nó dos Big Numbers, e só finaliza
*          quando não há mais nenhum Nó, tanto do Big Number x, ou do Big Number y, além de
*          não precisar somar o dígito de transporte (quando a soma feita entre dois
*          dígitos ultrapassa 10).
*/

BigNumber* sum_big_numbers(BigNumber *x, BigNumber *y) {
    BigNumber* result = create_big_number("");

    if (x->is_positive != y->is_positive) {
        printf("Vi aqui que os dois sinais estão trocados");
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

        return result;
    }

    return result;
}