#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "auxiliar.h"
#include "bignumber.h"


/*
* @brief Cria um Big Number.
*
* @param str_number String do número.
*
* @details A função adiciona cada Nó a medida que lê cada dígito da string, ou seja, a cada
*          iteração, um novo Nó é adicionado ao final do Big Number. Cada dígito lido da
*          string é convertido em inteiro, antes de ser usado para a criação de um Nó.
*
* @return O Big Number criado.
*/

BigNumber create_big_number(char *str_number) {
    BigNumber big_number = (BigNumber)malloc(sizeof(struct BigNumber));

    big_number->first_digit = NULL;
    big_number->last_digit = NULL;
    big_number->is_positive = true;
    big_number->num_digits = 0;

    int i = 0;

    if (str_number[i] == '-') {
        big_number->is_positive = false;
        i++;
    }

    while (str_number[i] != '\0') {
        int digit = str_number[i] - '0';

        add_node_to_big_number(big_number,digit,true);
        i++;
    }

    return big_number;
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

BigNumber sum_big_numbers(BigNumber x, BigNumber y) {
    BigNumber result = create_big_number("");

    if (x->is_positive != y->is_positive) {
        int comparison_big_numbers_modules = compare_big_numbers_modules(x, y);

        if (comparison_big_numbers_modules == 1) {
            result = switch_to_sum_or_subtraction("sub", x->is_positive, x, y, result);
        }

        else if (comparison_big_numbers_modules == -1) {
            result = switch_to_sum_or_subtraction("sub", y->is_positive, y, x, result);
        }

        else {
            add_node_to_big_number(result, 0, false);
            return result;
        }
    }

    else {
        if (x->is_positive == false && y->is_positive == false) {
            result->is_positive = false;
        }

        else {
            result->is_positive = true;
        }

        Node node_x = x->last_digit;
        Node node_y = y->last_digit;

        int carry_digit = 0;

        while (node_x != NULL || node_y != NULL || carry_digit > 0) {
            int digit_x, digit_y, sum, new_result_digit;

            digit_x = (node_x != NULL) ? node_x->digit : 0;
            digit_y = (node_y != NULL) ? node_y->digit : 0;

            sum = digit_x + digit_y + carry_digit;
            carry_digit = sum / 10;
            new_result_digit = sum % 10;

            add_node_to_big_number(result, new_result_digit, false);

            if (node_x != NULL) node_x = node_x->prev_digit;
            if (node_y != NULL) node_y = node_y->prev_digit;
        }
    }

    result->is_even = (result->last_digit->digit % 2 == 0) ? true : false;
    remove_zeros_from_left(result);

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

BigNumber subtract_big_numbers(BigNumber x, BigNumber y) {
    BigNumber result = create_big_number("");

    if (x->is_positive != y->is_positive) {
        result = switch_to_sum_or_subtraction("sum", x->is_positive, x, y, result);
    }

    else {
        Node node_x = x->last_digit;
        Node node_y = y->last_digit;

        bool result_sign = determine_sign_in_subtraction(x, y);
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
            }

            else {
                borrow_digit = 0;
            }

            add_node_to_big_number(result, subtraction, false);

            if (node_x != NULL) node_x = node_x->prev_digit;
            if (node_y != NULL) node_y = node_y->prev_digit;
        }

        result->is_positive = result_sign;
        result->is_even = (result->last_digit->digit % 2 == 0) ? true : false;
    }

    remove_zeros_from_left(result);

    return result;
}


/*
* @brief Divide dois Big Numbers.
*
* @param dividend Big Number que será dividido.
* @param divisor Big Number que será usado como divisor.
*
* @details A função realiza a divisão de dois Big Numbers utilizando um método similar
*          ao da divisão longa. Para cada dígito do dividendo, é verificado quantas
*          vezes o divisor cabe no valor parcial. O resultado é acumulado no quociente.
*          A função também valida divisões por zero e ajusta o sinal do resultado
*          com base nos sinais do dividendo e divisor.
*
* @return Big Number quociente da divisão.
*/

BigNumber divide_big_numbers(BigNumber dividend, BigNumber divisor) {
    bool result_sign = true ? dividend->is_positive == divisor->is_positive : false;

    dividend->is_positive = true;
    divisor->is_positive = true;

    BigNumber quocient = create_big_number("");
    BigNumber current_dividend = create_big_number("");

    Node dividend_node = dividend->first_digit;

    while (dividend_node != NULL) {
        add_node_to_big_number(current_dividend, dividend_node->digit, true);
        remove_zeros_from_left(current_dividend);

        if (divisor->first_digit->digit == 0 && divisor->first_digit == divisor->last_digit) {
            add_node_to_big_number(quocient, 0, true);
            return quocient;
        }

        int count = 0;

        while (compare_big_numbers_modules(current_dividend, divisor) >= 0) {
            BigNumber update_current = subtract_big_numbers(current_dividend, divisor);

            free_big_number(current_dividend);
            current_dividend = update_current;

            count++;
        }

        add_node_to_big_number(quocient, count, true);
        dividend_node = dividend_node->next_digit;
    }

    free_big_number(current_dividend);

    quocient->is_positive = result_sign;
    remove_zeros_from_left(quocient);

    return quocient;
}


/*
* @brief Multiplica dois Big Numbers.
*
* @param x Big Number a ser multiplicado.
* @param y Big Number a ser multiplicado.
*
* @details A função realiza a multiplicação de dois Big Numbers utilizando o algoritmo
*          tradicional de multiplicação. Cada dígito do primeiro número é multiplicado
*          pelos dígitos do segundo número, acumulando os resultados de forma apropriada.
*          O sinal do resultado é determinado com base nos sinais dos números multiplicados.
*
* @return Big Number resultado da multiplicação.
*/

BigNumber multiply_big_numbers(BigNumber x, BigNumber y) {
    BigNumber result = create_big_number("");

    bool result_sign = true ? x->is_positive == y->is_positive : false;
    int result_length = x->num_digits + y->num_digits;

    for (int i = 0; i < result_length; i++) {
        add_node_to_big_number(result, 0, false);
    }

    Node result_extremity = result->last_digit;

    for (Node i = x->last_digit; i; i = i->prev_digit) {
        Node current_decimal_magnitude = result_extremity;

        for (Node j = y->last_digit; j; j = j->prev_digit) {
            int product = (i->digit * j->digit) + current_decimal_magnitude->digit;

            current_decimal_magnitude->digit = product % 10;

            current_decimal_magnitude = current_decimal_magnitude->prev_digit;
            current_decimal_magnitude->digit += product / 10;
        }

        result_extremity = result_extremity->prev_digit;
    }

    remove_zeros_from_left(result);
    result->is_positive = result_sign;

    return(result);
}


/*
* @brief Calcula a exponenciação rápida de um Big Number.
*
* @param base Big Number que será elevado à potência.
* @param exponent Big Number que será usado como expoente.
*
* @details A função implementa o algoritmo de exponenciação rápida para calcular
*          potências de maneira eficiente. Se o expoente for zero, o resultado é 1.
*          Caso o expoente seja par, ele é dividido por 2, e a base é multiplicada
*          por si mesma. Se o expoente for ímpar, o resultado é obtido multiplicando-se
*          a base pelo resultado da exponenciação com o expoente reduzido em 1.
*          O algoritmo utiliza recursão para realizar os cálculos e libera a memória
*          alocada dinamicamente para Big Numbers intermediários.
*
* @return Big Number resultado da exponenciação.
*/

BigNumber fast_exponentiation(BigNumber base, BigNumber exponent) {
    if (exponent->first_digit->digit == 0 && exponent->first_digit == exponent->last_digit) {
        BigNumber result = create_big_number("");
        add_node_to_big_number(result, 1, true);
        return result;
    }

    else if (exponent->is_even){
        BigNumber two = create_big_number("2");
        BigNumber exponent_divided_by_2 = divide_big_numbers(exponent, two);
        BigNumber half_power = fast_exponentiation(base, exponent_divided_by_2);
        BigNumber result = multiply_big_numbers(half_power, half_power);

        free_big_number(two);
        free_big_number(exponent_divided_by_2);
        free_big_number(half_power);

        return result;
    }

    else if (!exponent->is_even) {
        BigNumber one = create_big_number("1");
        BigNumber exponent_minus_1 = subtract_big_numbers(exponent, one);
        BigNumber partial_result = fast_exponentiation(base, exponent_minus_1);
        BigNumber result = multiply_big_numbers(base, partial_result);

        free_big_number(one);
        free_big_number(exponent_minus_1);
        free_big_number(partial_result);

        return result;
    }

    return NULL;
}


/*
* @brief Calcula o resto da divisão entre dois Big Numbers.
*
* @param dividend Big Number dividendo.
* @param divisor Big Number divisor.
*
* @details A função calcula o resto da divisão utilizando a fórmula: resto = dividendo -
*          (quociente * divisor). O sinal do resultado é ajustado com base no divisor e
*          no dividendo, garantindo a consistência matemática.
*
* @return Big Number resto da divisão.
*/

BigNumber remainder_of_division(BigNumber dividend, BigNumber divisor) {
    bool divisor_sign = divisor->is_positive;
    bool dividend_sign = dividend->is_positive;

    BigNumber quocient = divide_big_numbers(dividend, divisor);

    divisor->is_positive = divisor_sign;
    BigNumber quocient_times_divisor = multiply_big_numbers(quocient, divisor);

    dividend->is_positive = dividend_sign;
    BigNumber remainder = subtract_big_numbers(dividend, quocient_times_divisor);

    if ((remainder->is_positive == false && divisor->is_positive == true) ||
        (remainder->is_positive == true && divisor->is_positive == false)) {
        BigNumber update_remainder = sum_big_numbers(remainder, divisor);

        free_big_number(remainder);
        remainder = update_remainder;
    }

    free_big_number(quocient);
    free_big_number(quocient_times_divisor);

    return remainder;
}


/*
* @brief Multiplica dois Big Numbers utilizando o algoritmo de Karatsuba.
*
* @param x Big Number a ser multiplicado.
* @param y Big Number a ser multiplicado.
*
* @details Implementa o algoritmo de Karatsuba para multiplicação eficiente de números.
*          Divide os números em partes, realiza multiplicações menores e combina os
*          resultados de forma eficiente. O algoritmo é mais rápido que o método
*          tradicional para números grandes.
*
* @return Big Number resultado da multiplicação.
*/

BigNumber multiply_karatsuba_big_numbers(BigNumber x, BigNumber y) {
    BigNumber result;

    bool result_sign = true ? x->is_positive == y->is_positive : false;

    x->is_positive = true;
    y->is_positive = true;

    if (x->num_digits <= 3 && y->num_digits <= 3) {
        result = multiply_big_numbers(x, y);
    }

    else {
        int tam = (x->num_digits > y->num_digits) ? x->num_digits : y->num_digits;
        int half = ceil(tam / 2.0);

        BigNumber x_left = divide_by_power_of_ten(x, half);
        BigNumber x_right = get_remainder_by_power_of_ten(x, half);

        BigNumber y_left = divide_by_power_of_ten(y, half);
        BigNumber y_right = get_remainder_by_power_of_ten(y, half);

        BigNumber a = multiply_karatsuba_big_numbers(x_left, y_left);
        BigNumber b = multiply_karatsuba_big_numbers(x_right, y_right);

        BigNumber sum_x_parts = sum_big_numbers(x_left, x_right);
        BigNumber sum_y_parts = sum_big_numbers(y_left, y_right);

        BigNumber c = multiply_karatsuba_big_numbers(sum_x_parts, sum_y_parts);
        BigNumber d = subtract_big_numbers(c, a);
        BigNumber d_temp = subtract_big_numbers(d, b);

        free_big_number(d);

        d = d_temp;

        int count = 0;

        while (count < half * 2){
            add_node_to_big_number(a, 0, true);

            if (count < half){
                add_node_to_big_number(d, 0, true);
            }

            count++;
        }

        result = a;
        BigNumber result_aux = sum_big_numbers(result, d);
        free_big_number(result);

        result = sum_big_numbers(result_aux, b);

        free_big_number(b);
        free_big_number(c);
        free_big_number(d);
        free_big_number(x_left);
        free_big_number(x_right);
        free_big_number(y_left);
        free_big_number(y_right);
        free_big_number(sum_x_parts);
        free_big_number(sum_y_parts);
        free_big_number(result_aux);
    }

    result->is_positive = result_sign;
    return result;
}


/*
* @brief Realiza o print de um Big Number.
*
* @param big_number Big Number a ser printado.
*/

void print_big_number(BigNumber big_number) {
    if ((big_number->is_positive == false)) printf("-");

    Node current_node = big_number->first_digit;

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

void free_big_number(BigNumber big_number) {
    Node current_node = big_number->first_digit;

    while (current_node != NULL) {
        Node next_node = current_node->next_digit;
        free(current_node);
        current_node = next_node;
    }

    free(big_number);
}