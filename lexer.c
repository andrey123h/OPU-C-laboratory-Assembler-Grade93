#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "errors.h"
#include "ass_globals.h"
#include "lexer.h"
#include <stdlib.h>
#include "file_handle.h"
#include "conversions.h"
#include "data_struct.h"

/* -------- data related functions -------- */

int valid_data(char* line, int line_num) {
    int func_flag;
    char* start; /* first char */
    char* end; /* last char */
    func_flag = 0;
    start = strstr(line, ".data") +5; /* to get past the .data */
    /* check if first char is a comma */
    if (*start == ',') {
        printf("line: %i ", line_num);
        print_error_msg(20);
        func_flag = 1;
    }
    trim_trailing_whitespace(line); /* for the last char check */
    end = line + strlen(line)-1;
    /* check of last char is a comma */
    if (*end == ',') {
        printf("line: %i ", line_num);
        print_error_msg(20);
        func_flag = 1;
    }
    /* check for integers only */
    while (*start) {
        /* skip  plus/minus/comma */
        if (*start == '-' || *start == '+' || *start == ',') {
            start++;
            continue;
        }
        if (!isdigit(*start)) {
            printf("line: %i ", line_num);
            print_error_msg(20);
            func_flag = 1;
            break;
        }
        start++;
    }
    return func_flag;  /* 0 for success */
}

int count_integers(char* line) {
    char* start; /* first char */
    int count;
    start = strstr(line, ".data") +5;
    count = 0;
    while (*start) {
        if (*start == '-' || *start == '+' || *start == ',') {
            start++;
            continue;
        }
        /* when digit is found, skip to the next comma or end of the string */
        if (isdigit(*start)) {
            count++;
           /* skip all digits of the current number */
            while (isdigit(*start)) {
                start++;
            }
        }
        else start++;
    }
    return count;
}

void convert_add_integers(data_list** head, char* line) {
    char* start;
    char number_str[6];  /* buffer to store number up to 6 digits */
    int i;
    int number;
    char* binary_str; /* binary represantion of an int */
    start = strstr(line, ".data") + 5; /* to get past the .data */
    while (*start) {
        i = 0;
        /* Skip commas */
        if (*start == ',') {
            start++;
            continue;
        }
        /* optional sign */
        if (*start == '-' || *start == '+') {
            number_str[i++] = *start++;
        }
        /* get digits of the current number */
        while (isdigit(*start)) {
            number_str[i++] = *start++;
        }
        if (i > 0) {
            number_str[i] = '\0';
            number = atoi(number_str);
            /* convert the integer to binary */
             binary_str = bits_convertor(number, WORD_SIZE);
            add_data(head,binary_str); /* add to linked list of data */
            free(binary_str);  /* free the binary string not needed anymore */
        }
        if (*start == ',') { /*  move to the next char after comma */
            start++;
        }
    }
}


int valid_string(char* line, int line_num) {
    int quote_count;
    char *start_quote;
    char *end_quote;
    char *ptr; /* pointer to line chars */
    ptr = line;
    quote_count = 0; /* number of quotes */
    start_quote = strchr(line, '"'); /* first qoute in line */
    end_quote = strrchr(line, '"');  /* last qoute */
    /* count num of quotes */
    while (*ptr) {
        if (*ptr == '"') {
            quote_count++;
        }
        ptr++;
    }
    /* if qoutes are not placed correcty or not 2 */
    if (start_quote + 1 == end_quote || quote_count != 2) {
        printf("line: %i  ", line_num);
        print_error_msg(19);
        return 1;
    }

    return 0; /* success */
}

void convert_add_chars(data_list** head, char* line) {
    char char_str[2]; /* buffer to store character with null terminator string */
    char* null_bnr;
    char* binary_str;
    char* start;
    char* end;
    start = strchr(line, '"');
    start++; /* move past the '"' */
     end = strchr(start, '"');

    /* iterate each character between the opening and closing double quotes. */
    while (start < end) {
        char_str[0] = *start; /* Copy the current character into the buffer. */
        char_str[1] = '\0';
        /* convert to binary string */
        binary_str = bits_convertor(char_str[0], WORD_SIZE);
        add_data(head, binary_str); /* add to the data list */

        free(binary_str); /* free the binary string not needed anymore */
        start++; /* move to the next char in the string. */
    }
    null_bnr = bits_convertor(0,WORD_SIZE);
    add_data(head,null_bnr);
}


int count_chars(char* line) {
    char *start_ch;
    char *end_ch;
    int count;
    count=0;
    start_ch = strchr(line, '"') ; /* first char */
    end_ch = strrchr(line, '"') ; /* last char  */
    /*  chars amount including the null terminator */
    count = (end_ch - start_ch) ;
    return count;
}

 /*-------- machinde code related functions -------- */

/* array of opcodes structures */
Opcode opcodes[] = {
    {"mov",2},
    {"cmp",2},
    {"add",2},
    {"sub",2},
    {"lea",2},
    {"clr",1},
    {"not",1},
    {"inc",1},
    {"dec",1},
    {"jmp",1},
    {"bne",1},
    {"red",1},
    {"prn",1},
    {"jsr",1},
    {"rts",0},
    {"stop",0},
};

/* valid opcode opnames for each addressing method */
char* direct_addressing_opnames[] = {"mov", "cmp", "add", "sub", "prn"};
char *label_addressing_opnames[] = {"mov", "cmp", "add", "sub", "prn", "lea", "clr", "not",
                                   "inc", "dec", "jmp", "bne", "red", "jsr"};
char* direct_reg_opnames[] = {"mov","cmp","add","sub","lea","clr","not","inc","dec",
                                "red", "prn"};
char* indirect_reg_opnames[] ={"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp",
                                "bne","red", "prn", "jsr"};

int search_opcode(char *name) {
    int i = 0;
    for (; i < OPCODES_COUNT; i++) {
        if (strcmp(opcodes[i].opcode_name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int valid_direct_addressing(char* opname,char* operand, int line_number) {
    int i;
    int valid_method;
    int valid_int;
    valid_int = 0; /* valid int flag. 0 if valid */
    valid_method = 0; /* method flag. 1 if valid */
     i = 0;
    operand++; /* move past "#" char */
    /* checks for valid integer */
    if (*operand == '-' || *operand == '+') { /* Check for optional sign */
        operand++; /* move past the sign */
    }
    if (!*operand) {
        valid_int = 1; /* if there is nothing after sing or # */
        printf("line: %i", line_number);
        print_error_msg(15);
    }
    while (*operand) {
        if (!isdigit(*operand)) {
            valid_int = 1; /* if non digit char was found */
            printf("line: %i", line_number);
            print_error_msg(14);
            break;
        }
        operand++;
    }
    for (; i < direct_opnames_count; i++) {
        if (strcmp(opname, direct_addressing_opnames[i]) == 0) {
            valid_method = 1;
        }
    }
    if (!valid_method) {
        printf("line: %i", line_number);
        print_error_msg(11);
    }
    if (valid_method != 1 || valid_int != 0) return 1;
    return 0; /* success */

}

int valid_label_addressing(char* opname, int line_number) {
    int valid;
    int i;
    valid = 0;
    i =0;
    for (; i < label_opnames_count; i++) {
        if (strcmp(opname, label_addressing_opnames[i]) == 0) {
            valid = 1;
            break;
        }
    }
    if (!valid) {
        printf("line: %i", line_number);
        print_error_msg(11);
    }
    if (!valid) return 1;
    return 0;
}

/* no need to check valid register num as the check already done */
int valid_direct_reg_addressing(char* opname, int line_number) {
    int valid_method;
    int i;
    i = 0;
    valid_method = 0; /* 1 for valid */
    for (; i < dir_reg_opnames_count; i++) {
        if (strcmp(opname, direct_reg_opnames[i]) == 0) {
            valid_method = 1;
            break;
        }
    }
    if (!valid_method) {
        printf("line: %i", line_number);
        print_error_msg(11);
    }
    if (valid_method != 1) return 1;
    return 0;
}

int valid_indirect_reg_addressing(char* opname,char* operand, int line_number) {
    int valid_method;
    int i;
    int valid_reg;
    valid_reg = 0; /* 0 if valid */
    valid_method = 0; /* 1 if valid */
    i =0;
    operand++; /* move past the "*" char */
    /* check for valid reg */
    if(is_reg(operand) !=1 ) {
        printf("line: %i", line_number);
        print_error_msg(12);
        valid_reg = 1;
    }
    for (; i < indir_reg_opnames_count; i++) {
        if (strcmp(opname, indirect_reg_opnames[i]) == 0) {
            valid_method = 1;
            break;
        }
    }
    if (!valid_method) {
        printf("line: %i", line_number);
        print_error_msg(11);
    }
    if (valid_method != 1 ||valid_reg != 0) return 1;
    return 0;
}

int valid_empty(char* opname, int line_number) {
    int valid_method;
    valid_method = 1; /* 1 if valid */
     if (strcmp(opname, "stop") != 0 && strcmp(opname, "rts") != 0){
        printf("line: %i", line_number);
        print_error_msg(11);
         valid_method = 0;
    }
    if (valid_method != 1) return 1;
    return 0;
}

int check_syntax_error(FILE *file) {
    int func_error;
    char line[MAX_LINE_LENGTH];
    int line_num;
    int line_error; /* each line flag */
    char *colon_pos; /* colon pointer */
    int valid_start; /* flag for valid start; with opname label or directive */
    int i;
    char *line_p;
    func_error = 0; /* func success flag */
    line_num = 0;
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        line_num++;
        if (strcmp(line, "\n") == 0) {
            continue;
        }
        line_error = 0;
        colon_pos = strchr(line, ':');
        line_p = line;
        i = 0;
        valid_start = 0;
        /* if line starts with a label and ensuring colon not the first character */
        if (colon_pos != NULL) {
            /* Check if everything before the colon is alphanumeric */
            for (; line_p < colon_pos; line_p++) {
                if (!isalnum(*line_p)) {
                    printf("line: %i", line_num);
                    print_error_msg(17);
                    line_error = 1;
                }
            }
        } else { /* proceed to check for a valid opcode or directive */
            for (; i < OPCODES_COUNT; i++) {
                if (strncmp(line, opcodes[i].opcode_name, strlen(opcodes[i].opcode_name)) == 0) {
                    valid_start = 1;  /* Line starts with a valid opcode */
                    break;
                }
            }
            /* check if the line starts with valid directive */
            if (!valid_start &&
                (strncmp(line, ".string", strlen(".string")) == 0 ||
                 strncmp(line, ".data", strlen(".data")) == 0 ||
                strncmp(line, ".entry", strlen(".entry")) == 0 ||
                strncmp(line, ".extern", strlen(".extern")) == 0)) {
                valid_start = 1;
     }
            if (!valid_start) {
                line_error = 1;
            }
        }
        if (line_error) {
            printf("line: %i", line_num);
            print_error_msg(18);
            func_error = 1;
        }
    }
    rewind(file); /* rewind for the first pass */
    return func_error; /* 0 for success */
}
