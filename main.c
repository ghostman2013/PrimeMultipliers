#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>

#define ARRAY_INC 12
//Определяем новый тип для кратости
typedef unsigned long long ULONG;

// Выводит ошибку в стандартный поток вывода
void print_err() {
    printf("[error]");
}

// Возвращает следующее простое число, начиная с некоторого исходного
void inc_prime(ULONG* prime) {
    ULONG next = *prime;
    char flag;

    do {
        ++next;
        flag = 1;
        // Выполняем проверку числа на простоту
        for (ULONG i = 2; i*i <= next; ++i) {

            if (next % i == 0) {
                flag = -1;
                break;
            }
        }
    } while (flag == -1);

    *prime = next;
}

// Выводит элементы массива
int print_arr(ULONG* start, ULONG* end) {

    if (start == NULL || end < start) {
        return -1;
    }

    for (ULONG* i = start; i < end; ++i) {
        printf("%llu ", *i);
    }

    printf("%llu", *end);

    return 0;
}

// Проверяет корректность ввода
int is_unum(const char *s)
{
    int count = 0;

    while ((*s != '\0') && (*s != '\n')) {

        if (isdigit(*s++) == 0) {
            return 0;
        }

        ++count;
    }

    if (count > 20) {
        return 0;
    }

    return 1;
}

// Считывает число из стандартного потока ввода
int scan_num(ULONG* num) {
    char str[100];
    char* fgets_res = fgets(str, sizeof(str), stdin);

    if (*fgets_res == EOF || !is_unum(str)) {
        return 0;
    }

    char* end_ptr;
    errno = 0;
    *num = strtoull(str, &end_ptr, 10);

    if (errno != 0 || end_ptr == str || *num == 0) {
        return 0;
    }

    return 1;
}

int add_element(ULONG** arr, ULONG num, int* size, int* capacity) {

    if ((*size) == (*capacity)) {
        *capacity += ARRAY_INC;
        *arr = (ULONG*)realloc(*arr, (*capacity) * sizeof(ULONG));

        if (*arr == NULL) {
            return 1;
        }
    }

    *(*arr + ((*size)++)) = num;

    return 0;
}

// Возвращает массив простых множителей, последний элемент
// которого 0, а первый 1.
int find_pmuls(ULONG src_num, ULONG** p_muls, int* size) {
    int capacity = ARRAY_INC; // Размер массива
    *size = 1; // Количество элементов
    ULONG* arr = (ULONG*)malloc(capacity * sizeof(ULONG));
    arr[0] = 1;
    ULONG divider = 2;

    while (divider <= src_num) {

        while (src_num % divider == 0) {
            src_num /= divider;

            if (add_element(&arr, divider, size, &capacity)) {
                return 1;
            }
        }

        inc_prime(&divider);
    }

    if (src_num != 1) {

        if (add_element(&arr, divider, size, &capacity)) {
            return 1;
        }
    }

    *p_muls = (ULONG*)malloc((*size + 1) * sizeof(ULONG));
    memcpy(*p_muls, arr, (*size) * sizeof(ULONG));
    *(*p_muls + ((*size)++)) = 0;

    free(arr);
    arr = NULL;

    return 0;
}

int main()
{
    ULONG src;
    // Проверяем корректность ввода, если нет,
    // выводим ошибку и завершаем программу
    if (!scan_num(&src)) {
        print_err();
        return 0;
    }

    ULONG* p_muls = NULL;
    int size = 0;
    // Находим все простые множители числа
    if (find_pmuls(src, &p_muls, &size)) {
        print_err();
        return 0;
    }
    // Выводим массив в стандартный поток вывода
    if(print_arr(p_muls, p_muls + size - 2) != 0) {
        print_err();
        return 0;
    }
    // Освобождаем память под массив простых множителей
    free(p_muls);
    p_muls = NULL;

    return 0;
}
