#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "file_handle.h"
#include <ctype.h>
#include "ass_globals.h"


/* list of known characters */
char *OPNAMES_CHECK[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
                  "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
char *REGS_CHECK[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
char *INDIRECT_REGS_CHECK[] = {"*r0", "*r1", "*r2", "*r3", "*r4", "*r5", "*r6", "*r7"};
char *INSTUCTIONS_CHECK[] = {".data", ".string", ".extern", ".entry"};


int is_opname(char *str) {
    int i;
    if (str == NULL) {
        return 0;
    }
    for (i = 0; i < OPCODES_COUNT_CHEK; i++) {
        if (strcmp(str, OPNAMES_CHECK[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_reg(char *str) {
    int i;
    if (str == NULL) {
        return 0;
    }
    for (i = 0; i < REG_COUNT_CHEK; i++) {
        if (strcmp(str, REGS_CHECK[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

/* for indirect register name for macro name */
int is_indirect_reg(char *str) {
    int i;
    if (str == NULL) {
        return 0;
    }
    for (i = 0; i < INDIRECT_REGS_COUNT_CHECK; i++) {
        if (strcmp(str, INDIRECT_REGS_CHECK[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_instruction(char *str) {
    int i;
    if (str == NULL) {
        return 0;
    }
    for (i = 0; i < INSTRUCTIONS_COUNT_CHEK; i++) {
        if (strcmp(str, INSTUCTIONS_CHECK[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

char *new_file(char *name, char *extension) {
    char *dot; /* pointer to the first '."  character */
    char *new_name;
    new_name = malloc(MAX_FILE_LENGTH * sizeof(char));
    if (new_name == NULL) {
        print_error_msg(5);
        return NULL;
    }
    /* Copy the name to the allocated memory*/
    strcpy(new_name, name);
    /* Find the first '.' */
    if ((dot = strchr(new_name, '.')) != NULL) {
        *dot = '\0';
    }
    /* Append the extension to the new name */
    strcat(new_name, extension);
    return new_name;

}

char *remove_whitespaces(char *input_filename) {
    int ch;
    char *temp_file;
    FILE *input_file;
    FILE *output_file;
    input_file = fopen(input_filename, "r");
    /* Create a new filename with the ".firstproc" extension */
    temp_file = new_file(input_filename, ".firstproc");
    output_file = fopen(temp_file, "w");

    if (input_file == NULL || output_file == NULL) {
        print_error_msg(3);
        return NULL;
    }
    /* variable to hold each character read from the input file */

    /* If character is not a space and not a tab,write the character to the output file */
    while ((ch = fgetc(input_file)) != EOF) {
        if (ch != ' ' && ch != '\t') {
            fputc(ch, output_file);
        }
    }

    fclose(input_file);
    fclose(output_file);
    return temp_file;
}



/* Function to trim trailing whitespace from a string */
void trim_trailing_whitespace(char *str) {
    int len;
    len = strlen(str);
    while (len > 0 && isspace(str[len - 1])) {
        str[--len] = '\0';
    }
}


void copy_file(char *name_of_dest, char *name_of_src) {
    char str[MAX_LINE_LENGTH];
    FILE *fp, *fp_dest;
    fp = fopen(name_of_src, "r");
    if (fp == NULL) {
        print_error_msg(3);
    }
    fp_dest = fopen(name_of_dest, "w");
    if (fp_dest == NULL) {
        print_error_msg(3);
        fclose(fp);
    }
    while (fgets(str, MAX_LINE_LENGTH, fp) != NULL) {
        fprintf(fp_dest, "%s", str);
    }
    fclose(fp);
    fclose(fp_dest);

}