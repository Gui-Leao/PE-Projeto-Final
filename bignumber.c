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

Node create_node(int digit) {
    Node new_node = (Node)malloc(sizeof(struct Node));

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
    //is_even(big_number);
    printf("\n");
    //big_number->is_even?printf("é par\n"):printf("é impar\n");
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
        } else {
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

BigNumber subtraction_big_numbers(BigNumber x, BigNumber y) {
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
            } else {
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
            BigNumber update_current = subtraction_big_numbers(current_dividend, divisor);

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
        BigNumber exponent_minus_1 = subtraction_big_numbers(exponent, one);
        BigNumber partial_result = fast_exponentiation(base, exponent_minus_1);
        BigNumber result = multiply_big_numbers(base, partial_result);

        free_big_number(one);
        free_big_number(exponent_minus_1);
        free_big_number(partial_result);

        return result;
    }

    return NULL;
}

BigNumber multiply_karatsuba_big_numbers(BigNumber x, BigNumber y){
    BigNumber result = create_big_number(""); 

    //caso base para quando o big number tiver apenas um digito
    if (x->num_digits == 1 && y->num_digits == 1){
        result = multiply_big_numbers(x,y);
        // printf("mostrando o resultado: \n");
        // print_big_number(result);
        //printf("\n");
    }
    else{
        int tam = (x->num_digits>y->num_digits)?x->num_digits:y->num_digits;
        int half = tam / 2;
        // printf("tamanho : %d\n",tam);
        // printf("tamanho metade : %d\n",half);
        BigNumber base_ten = create_big_number("10");
        //print_big_number(base_ten);
        char* num_digits_str = create_big_number_str(half);
        BigNumber exponent = create_big_number(num_digits_str);
        //printf("Numero de digitos : %d\n", x->num_digits/2);

        BigNumber x_left = create_big_number("");
        BigNumber x_right = create_big_number("");
        BigNumber y_left = create_big_number("");
        BigNumber y_right = create_big_number("");

        BigNumber a = create_big_number("");
        BigNumber b = create_big_number("");
        BigNumber c = create_big_number("");
        BigNumber d = create_big_number("");
        int half_right = tam / 2;
        //int half_left = tam - half_right;

       


        x_left = divide_big_numbers(x,fast_exponentiation(base_ten,exponent));
        copy_big_number(x_right,x,half_right,true);
        //copy_big_number(y_left,y,tam,false);
        y_left = divide_big_numbers(y,fast_exponentiation(base_ten,exponent));
        copy_big_number(y_right,y,half_right,true);
        // printf("veio auqizn\n");

        // printf("*************************\n");
        // print_big_number(x_left);
        // print_big_number(x_right);
        // print_big_number(y_left);
        // print_big_number(y_right);
        // printf("*************************\n");

        a = multiply_karatsuba_big_numbers(x_left,y_left);
        b = multiply_karatsuba_big_numbers(x_right,y_right);
        c = multiply_karatsuba_big_numbers(sum_big_numbers(x_right,x_left),sum_big_numbers(y_right,y_left));
        d = subtraction_big_numbers(c,a);
        d = subtraction_big_numbers(d,b);

        // printf("imprimindo a : ");
        // print_big_number(a);
        // printf("\n");

        // printf("imprimindo b : ");
        // print_big_number(b);
        // printf("\n");

        // printf("imprimindo c : ");
        // print_big_number(c);
        // printf("\n");

        // printf("imprimindo d : ");
        // print_big_number(d);
        // printf("\n");


        free(num_digits_str);
        free_big_number(exponent);
        num_digits_str = create_big_number_str(tam);
        exponent = create_big_number(num_digits_str);

        result = multiply_big_numbers(a,fast_exponentiation(base_ten,exponent));
        // printf("Resultado primeira parte: ");
        // print_big_number(result);
        // printf("\n");
        free(num_digits_str);
        free_big_number(exponent);
        num_digits_str = create_big_number_str(half);
        exponent = create_big_number(num_digits_str);
        
        result = sum_big_numbers(result,multiply_big_numbers(d,fast_exponentiation(base_ten,exponent)));
        // printf("Resultado segunda parte: ");
        // print_big_number(result);
        // printf("\n");
        result = sum_big_numbers(result,b);
        // printf("Resultado terceira parte: ");
        // print_big_number(result);
        // printf("\n");
        //print_big_number(exponent);
        // base_ten = fast_exponentiation(base_ten,x->num_digits);

        
        // result = multiply_big_numbers(a,base_ten);
        // free_big_number(base)
        // result = sum
        //add_node_to_big_number(result,0,true); 

    }

    //printf("--------------------------\n");
    return result;

    // a = x_left*y_left 
    // b = y_rigth*x_right
    // d = (x_right*y_left +y_left*x_right)
    // c = (x_left + x_rigth) * (y_left + y_right)
    // d = c - a - b
    // a * 10^n + d * 10 ^(n/2) + b


}



