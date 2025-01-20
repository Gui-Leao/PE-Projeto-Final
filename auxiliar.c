#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "auxiliar.h"
#include "bignumber.h"

/* 
* @brief Lê a entrada fornecida pelo arquivo de teste.
*
* @details A função aloca dinamicamente na memória a string que está sendo fornecida
*          na entrada através do getchar(), identificando se é o fim do arquivo ou
*          o fim da linha.
*/

char* read_input() {
    int capacity = 16;
    int size = 0;
    char* input = malloc(capacity);

    int c;

    while ((c = getchar()) != EOF && c != '\n') {
        input[size] = c;
        size++;

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
* @brief Executa o programa.
*
* @details Aqui é usado a função read_input() pra alocar dinamicamente
*          as strings fornecidas dos Big Numbers e das operações.
*          Se observado que não há mais números sendo fornecidos para as operações,
*          o programa para.
*/

void execute_program() {
    while(1) {
        char* number_1 = read_input();
        
        if (strlen(number_1) == 0) {
            free(number_1);
            break;
        }

        char* number_2 = read_input();
        char* operation = read_input();

        BigNumber big_num1 = create_big_number(number_1);
        BigNumber big_num2 = create_big_number(number_2);
        BigNumber result = NULL;

        switch (*operation) {
            case '+':
                result = sum_big_numbers(big_num1, big_num2);
                break;
            case '-':
                result = subtraction_big_numbers(big_num1, big_num2);
                break;
            case '/':
                result = divide_big_numbers(big_num1, big_num2);
                break;
            case '*':
                result = multiply_big_numbers(big_num1,big_num2);
                break;
            case '^':
                result = fast_exponentiation(big_num1, big_num2);
                break;
            case ']':
                result = multiply_karatsuba_big_numbers(big_num1,big_num2);
                break;
            default:
                printf("Operacao nao conhecida\n");
                result = create_big_number("");
                break;
        }
        if (result == NULL) {
            perror("Erro ao alocar memória");
            exit(1);
        }

        print_big_number(result);

        free_big_number(big_num1);
        free_big_number(big_num2);
        free_big_number(result);
        free(number_1);
        free(number_2);
        free(operation);
    }
}

/* 
* @brief Adiciona um novo Nó em um Big Number.
*
* @param big_number Registro do tipo Big Number.
* @param digit Número que representa o Nó a ser criado.
* @param insert_at_end Indica se vamos adicionar o número no fim ou no começo do Big Number.
*
* @details A função atualiza os campos do Nó criado (dígito posterior e anterior),
*          além alocar o novo Nó na primeira posição do Big Number.
*/

void add_node_to_big_number(BigNumber big_number, int digit, bool insert_at_end) {
    Node new_node = (Node)malloc(sizeof(struct Node));
    
    if (!insert_at_end) {
        new_node->digit = digit;
        new_node->next_digit = big_number->first_digit;
        new_node->prev_digit = NULL;

        if (big_number->first_digit == NULL) big_number->last_digit = new_node;
        if (big_number->first_digit != NULL) big_number->first_digit->prev_digit = new_node;
            
        big_number->first_digit = new_node;
    } else {
        new_node->digit = digit;
        new_node->prev_digit = big_number->last_digit;
        new_node->next_digit = NULL;

        if (big_number->last_digit == NULL) big_number->first_digit = new_node;
        if (big_number->last_digit != NULL) big_number->last_digit->next_digit = new_node;
        big_number->is_even = (digit % 2 == 0) ? true : false;
            
        big_number->last_digit = new_node;       
    }
    big_number->num_digits++;
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

int compare_big_numbers_modules(BigNumber x, BigNumber y) {
    int len_x = x->num_digits;
    int len_y = y->num_digits;
    if (len_x > len_y) return 1;
    if (len_x < len_y) return -1;

    Node node_x = x->first_digit;
    Node node_y = y->first_digit;

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

BigNumber switch_to_sum_or_subtraction(char *switch_to, bool sign, BigNumber x, BigNumber y, BigNumber result) {
    bool result_sign = true ? sign == true : false;

    x->is_positive = true;
    y->is_positive = true;

    free_big_number(result);
    
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
* @return true, para resultado positivo.
* @return false, para resultado negativo.
*/

bool determine_sign_in_subtraction(BigNumber x, BigNumber y) {
    int comparison_big_numbers_modules = compare_big_numbers_modules(x, y);

    if (x->is_positive == false) {
        return true ? comparison_big_numbers_modules == -1 : false;
    }

    else if (x->is_positive == true) {
        return true ? comparison_big_numbers_modules == 1 : false;
    }

    return NULL;
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

void determine_order_of_subtraction(BigNumber x, Node* node_x, BigNumber y, Node* node_y, BigNumber result) {
    int comparison_big_numbers_modules = compare_big_numbers_modules(x, y);   

    if (comparison_big_numbers_modules == -1) {
        *node_x = y->last_digit;
        *node_y = x->last_digit;
    }

    else if (comparison_big_numbers_modules == 0) {
        add_node_to_big_number(result, 0, false);
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

void remove_zeros_from_left(BigNumber big_number) {
    while (big_number->first_digit->digit == 0 && big_number->first_digit != big_number->last_digit) {
        Node node_to_remove = big_number->first_digit;

        big_number->first_digit = node_to_remove->next_digit;
        big_number->first_digit->prev_digit = NULL;

        free(node_to_remove);
        big_number->num_digits--;
    }

    if (big_number->first_digit == big_number->last_digit && big_number->first_digit->digit == 0) {
        big_number->is_positive = true;
    }
}

char* create_big_number_str(int num_digits) {
    // Calcula o número de dígitos necessários
    int length = snprintf(NULL, 0, "%d", num_digits);
    
    // Aloca memória dinamicamente para a string
    char* num_digits_str = (char*)malloc((length + 1) * sizeof(char));
    
    if (num_digits_str == NULL) {
        // Lida com erro de alocação de memória
        perror("Erro ao alocar memória");
        exit(1);
    }

    // Preenche a string com o valor de num_digits
    snprintf(num_digits_str, length + 1, "%d", num_digits);

    return num_digits_str;
}

void copy_big_number(BigNumber big_number_dest,BigNumber big_number_orig,int tam,bool in_left){


    if (!in_left){
        Node node_to_cpy = big_number_orig->first_digit;
        for (int i = 0; i < tam ; i ++){
            add_node_to_big_number(big_number_dest,node_to_cpy->digit,true);
            node_to_cpy = node_to_cpy->next_digit;
        }
    }
    else {
        Node node_to_cpy = big_number_orig->last_digit;
        for (int i = 0; i < tam ; i ++){
            add_node_to_big_number(big_number_dest,node_to_cpy->digit,false);
            node_to_cpy = node_to_cpy->prev_digit;
        }        

    }
    //printf("terminou aqui\n");

}