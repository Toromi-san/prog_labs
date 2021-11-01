#include <stdio.h>
#include <string.h>
#include "header.h"

#define PATTERN_SIZE 17

int main() {
    int tbl[256];

    char str_ptr[PATTERN_SIZE];
    char str_txt[40];
    if (read_file(str_ptr, str_txt))
        return 0;

    int len_ptr = (int) strlen(str_ptr);
    int len_txt = (int) strlen(str_txt);

    /*Creating table of shifts*/
    for (int i = 0; i < 256; i++)
        tbl[i] = len_ptr;

    /*Filling the table with values*/
    for (int i = 0; i < len_ptr - 1; i++) {
        int num;
        /*Add shifts for char with negative numeric values*/
        if ((int) str_ptr[i] < 0)
            num = 256 + (int) str_ptr[i];
        else
            num = (int) str_ptr[i];
        tbl[num] = len_ptr - i - 1;
    }

    algorithm(len_ptr, len_txt, str_txt, str_ptr, tbl);
    return 0;
}
