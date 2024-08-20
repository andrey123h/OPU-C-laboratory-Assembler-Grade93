#include <stdio.h>
#include <string.h>
#include "macr_struct.h"
#include "errors.h"
#include "preprocess.h"
#include "file_handle.h"
#include "ass_globals.h"

/* functions to check valid macro declerations */

int valid_macro_names(FILE *file) {
    int firstproc_flag;
    char line[MAX_LINE_LENGTH];
    int line_num;
    char *macro_p; /* pointer for macro name */
    char *new_line;
    line_num = 0; /* counter to track the current line number */
    firstproc_flag = 0; /* flag for function success */
    while (fgets(line, sizeof(line), file)) {
        line_num++;
        /* skip lines containing "endmacr" */
        if (strstr(line, "endmacr") != NULL) {
            continue;
        }
        if (strstr(line, "macr") != NULL) {
            /* find the position of "macr" in line */
            macro_p = strstr(line, "macr");
            /* move past the "macr" */
            macro_p += strlen("macr");
            /* find the newline character starting from macro_p */
            new_line = strchr(macro_p, '\n');
            if (new_line != NULL) {
                /* replace with null terminator */
                *new_line = '\0';
            }
            /* check for the invalid names */
            if (is_opname(macro_p) || is_instruction(macro_p) || is_reg(macro_p) || is_indirect_reg(macro_p)) {
                printf("Line:%i ", line_num);
                print_error_msg(2);
                firstproc_flag = 1;
            }
        }
    }
    return firstproc_flag; /* 0 for succsess */
}

int valid_endmacr(FILE *file){
    int firstproc_flag;
    char line[MAX_LINE_LENGTH];
    int line_num;
    line_num = 0;
    firstproc_flag = 0;
    while (fgets(line, sizeof(line), file)) {
        line_num++;
        /* check if the line contains "endmacr" */
        if (strstr(line, "endmacr") != NULL) {
            /* chek If the line is not exactly "endmacr" */
            if (strcmp(line, "endmacr\n") != 0 && strcmp(line, "endmacr") != 0) {
                printf("Line:%i ", line_num);
                print_error_msg(1);
                firstproc_flag = 1;
            }
        }
    }
   return firstproc_flag; /* 0 for success */
}

int exe_validate_declarations(FILE *file) {
    int endmacr_valid;
    int macro_names_valid;
    rewind(file);  /* reset file pointer to the beginning of file */
    endmacr_valid = valid_endmacr(file);
    rewind(file);
     macro_names_valid = valid_macro_names(file);
    rewind(file);
    return endmacr_valid == 0 && macro_names_valid == 0;
}
/* actual preprocessor functions */

macro* add_macro_to_list(FILE *file) {
    /*rewind(file);*/
    int found;
    int line_num;
    int copy;
    macro* head = NULL;
    char *macro_p;
    char line[MAX_LINE_LENGTH];
    char body[MAX_LINE_LENGTH]; /* buffer to store macro body */
    char macro_name[MAX_LINE_LENGTH];  /* buffer to store macro name */
     copy = 0; /* indicates if copying lines */
    line_num = 0;
    found = 0;/* indicates if the macro was found or not */
    while (fgets(line, sizeof(line), file)) {
        line_num++;
        if (strstr(line, "endmacr") != NULL) {
            copy = 0; /* stop copying when reached endmacr */
            /* add only valid macro names and bodies, and if not already exist */
            if (macro_name[0] != '\0' && body[0] != '\0') {
                if (search_macro(head, macro_name, &found) == NULL) {
                    add_macro(&head, macro_name, body);
                    /*printf("Added macro: %s\n", macro_name); debbuging only */
                }
                else {
                    printf("%s", macro_name);
                    print_error_msg(8);  /* duplicate macro define */
                }
                /* clear the previous values */
                macro_name[0] = '\0';
                body[0] = '\0';
            } else {
                printf("Line:%i ", line_num);
                print_error_msg(6);
                return NULL;
            }
            continue;
        }
        /* find the position of "macr" in the line and move past it */
        if (strncmp(line, "macr", 4) == 0) {
            macro_p = strstr(line, "macr");
            macro_p += strlen("macr");
            /* copy the macro name to buffer */
            strncpy(macro_name, macro_p, sizeof(macro_name) - 1);
            /* ensure null termination */
            macro_name[sizeof(macro_name) - 1] = '\0';
            body[0] = '\0';
            copy = 1;
            continue;
        }
        if (copy) {
            strcat(body, line); /* Add the line to the macro body buffer */
        }
    }
   /* display_macros(head); debbuging only */
    return head;
}

char* declaration_removal(char *file) {
    int macro_decl;
    char line[MAX_LINE_LENGTH];
    FILE *input_file;
    FILE *output_file;
    char* temp_file;
    macro_decl = 0; /* flag to track if inside macro decleration  */

    input_file = fopen(file, "r");
    if (input_file == NULL) {
        print_error_msg(3);
        return NULL;
    }
    /* create a new filename with the ".preproc" extension */
    temp_file = new_file(file, ".preproc");
    output_file = fopen(temp_file, "w");
    if (output_file == NULL) {
        print_error_msg(3);
        return NULL;
    }

    while (fgets(line, MAX_LINE_LENGTH, input_file)) {
        /* chek for decleration start */
        if (strncmp(line, "macr", 4) == 0) {
            macro_decl = 1; /* start skipping lines */

        }
        /* chek for decleration stop */
        if (strstr(line, "endmacr") != NULL) {
            macro_decl = 0; /* stop skipping lines */
            continue;
        }
        /* write to file if not part of an macro decleration */
        /* also cheks for comment line starting with ';' */
        if (macro_decl == 0 && strstr(line, ";") == NULL) {
            fprintf(output_file, "%s", line);
        }
    }
    fclose(input_file);
    fclose(output_file);
    return temp_file;
}

char *replace_names_with_body(char *file, macro *head) {
    char line[MAX_LINE_LENGTH];
    FILE *input_file;
    FILE *output_file;
    char* temp_file;
    int replaced;
    macro *current; /* pointer to the macro linked list nodes */
    input_file = fopen(file, "r");
    /* Create a new filename with the ".am" extension */
    temp_file = new_file(file, ".am");
    output_file = fopen(temp_file, "w");
    if (input_file == NULL || output_file == NULL) {
        print_error_msg(3);
        return NULL;
    }
    while (fgets(line, sizeof(line), input_file)) {
        current = head;
        replaced = 0; /* indicates for replacment */
        /* chek if the current line contains a macro name from the list */
        while (current != NULL) {
            if (strstr(line, current->name) != NULL) {
                /* replace line with macro body */
                fprintf(output_file, "%s" ,current->body);
                replaced = 1;
                break;
            }
            current = current->next;
        }
        /* write the original line to the output file */
        if (!replaced) {
            fprintf(output_file, "%s", line);
        }
    }
    fclose(input_file);
    fclose(output_file);
    return temp_file;
}
