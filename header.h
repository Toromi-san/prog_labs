#ifndef HEADER_H
#define HEADER_H
#include <stdio.h>
#include <string.h>

#define LAST_PATTERN_INDEX len_tmp - 1
#define INPUT_FILE "in.txt"
#define OUTPUT_FILE "out.txt"

void create_str_ptr(char *line, char *pattern) {
    /* :Action: Save useful char from first line of file to "pattern" string.
     * :Annotation: Some char values in "C" encoded with two bytes, for example one cyrillic symbol
     *              takes 2 bytes: first with decimal representation: -47 or -48 and second
     *              with decimal representation depending on the symbol.
     *              The second bytes will be saved in strings for further comparison*/
    int index = 0;
    for (int i = 0; i < strlen(line); i++)
        if (line[i] == '\n')
            pattern[index] = '\0';
        else
            if (line[i] != '╤' && (int)line[i] != -47) {
                pattern[index] = line[i];
                index++;
            }
}


void create_str_txt(char *line, char *text, int *index) {
    /* :Action: Save useful char from first line of file to "text" string.*/
    for (int i = 0; i < strlen(line); i++)
        if ((int) line[i] != -48 && (int) line[i] != -47) {
            text[*index] = line[i];
            *index += 1;
        }
}


int read_file(char *pattern, char *text) {
    /* :Action: Open file, if it exists and get pattern and text from it.
     * :args: {lines_num} counter of lines in file
     *        {index} index of symbol in pattern or text*/
    char line[40];
    int lines_num = 0;
    int index = 0;
    FILE *f_in = fopen(INPUT_FILE, "r");
    if (f_in == NULL)
        return 1;
    while (fgets(line, 40, f_in) != NULL) {
        lines_num++;
        /*First line from file save in string "pattern" */
        if (lines_num == 1)
            create_str_ptr(line, pattern);
        /*All symbols after the first line from file save in string "text"*/
        else
            create_str_txt(line, text, &index);
    }
    text[index] = '\0';
    fclose(f_in);
    return 0;
}


void make_shift(int *counter, const char *str_txt, int *cur_txt, int shift, const int *tbl, int *index) {
    /* :Action: Shift the cursor in the text
     * :args: {shift} number from table of shifts, by the value of which the cursor is shifted*/
    if (*counter == 0) {
        if ((int) str_txt[*cur_txt] < 0)
            shift = tbl[256 + (int) str_txt[*cur_txt]];
        else
            shift = tbl[(int) str_txt[*cur_txt]];
        *index = *cur_txt;
        *cur_txt = shift + *index;
    }
    else {
        if ((int) str_txt[*index] < 0)
            shift = tbl[256 + (int) str_txt[*index]];
        else
            shift = tbl[(int) str_txt[*index]];
        *cur_txt = shift + *index;
        *counter = 0;
    }
}


void algorithm(int len_tmp, int len_txt, const char *str_txt, const char *str_ptr, const int *tbl) {
    /* :Action: Search for a substring in the text about the algorithm of Boyer-Moor.
     * :args: {counter} value, which count coincidences of symbols, while pattern don't move
     *        {curr_text} index of symbol from str_text
     *        {index} index of last symbol of pattern in the text */
    int cur_txt = LAST_PATTERN_INDEX;
    int cur_ptr = LAST_PATTERN_INDEX;
    int shift = 0;
    int counter = 0;
    int index = LAST_PATTERN_INDEX;

    FILE *f_out = fopen(OUTPUT_FILE, "a");
    while (cur_txt < len_txt) {
        fprintf(f_out,"%d ", cur_txt + 1);
        if (str_txt[cur_txt] == str_ptr[cur_ptr]) {
            counter++;
            if (counter == 1)
                index = cur_txt;
            cur_ptr--;
            cur_txt--;

            /*Condition, which help to find other substrings in the text */
            if (counter == len_tmp) {
                counter = 0;
                cur_ptr = LAST_PATTERN_INDEX;
                cur_txt = index + len_tmp;
            }
        }
        else {
            make_shift(&counter, str_txt, &cur_txt, shift, tbl, &index);
            cur_ptr = LAST_PATTERN_INDEX;
        }
    }
    fclose(f_out);
}

#endif
