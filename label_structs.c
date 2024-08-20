#include "label_structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "ass_globals.h"
#include "conversions.h"
#include "file_handle.h"


label* create_label(char *name, int am_line_num, char* dec_address, int
    is_extern,  int is_entry) {
    label *new_label;
    new_label = malloc(sizeof(label));
    new_label->labelName = malloc(strlen(name) + 1);
    strcpy(new_label->labelName, name);
    new_label->am_line = am_line_num;
    new_label->dec_address = dec_address;
    new_label->IsExtern  = is_extern;
    new_label->IsEntry = is_entry;
    new_label->next = NULL;
    if (new_label->labelName == NULL) {
        print_error_msg(5);
        return NULL;
    }
    return new_label;
}

void add_label(label **head, char *name,int am_line_num,char* dec_address, int
    is_extern, int is_entry) {
    label *new_label;
    new_label = create_label(name, am_line_num,dec_address,is_extern,is_entry);
    if (*head == NULL) {
        *head = new_label;
    } else {
        label *i = *head;
        while (i->next != NULL) {
            i = i->next;
        }
        i->next = new_label;
    }
}


/* extern labels list create and add func */
extern_label* create_extern_label(char *name, int mention) {
    extern_label *new_extern_label;
    new_extern_label = malloc(sizeof(extern_label));
    new_extern_label->externName = malloc(strlen(name) + 1);
    if (new_extern_label->externName == NULL) {
       print_error_msg(5);
        return NULL;
    }
    strcpy(new_extern_label->externName, name);
    new_extern_label->extern_mention = mention;
    new_extern_label->next = NULL;
    return new_extern_label;
}

void add_extern_label(extern_label **head, char *name, int mention) {
    extern_label *new_extern_label;
    new_extern_label = create_extern_label(name, mention);
    if (*head == NULL) {
        *head = new_extern_label;
    } else {
        extern_label *i = *head;
        while (i->next != NULL) {
            i = i->next;
        }
        i->next = new_extern_label;
    }
}

/* debbugging only */
void print_extern_labels(extern_label *head) {
    extern_label *current;
    current = head;

    if (current == NULL) {
        printf("No extern labels to display.\n");
        return;
    }

    while (current != NULL) {
        printf("Extern Name: %s, Mention Address: %04d\n", current->externName, current->extern_mention);
        current = current->next;
    }
}

void release_extern_labels(extern_label *head) {
    extern_label *current;
    extern_label *next_extern_label;

    current = head;
    while (current != NULL) {
        next_extern_label = current->next;
        free(current->externName);
        free(current);
        current = next_extern_label;
    }
}


int valid_label_name(label *label_head, macro *macro_head) {
    int valid_flag;
    label *label_current;
    macro *macro_current;
    label_current = label_head;
    macro_current = NULL;
    valid_flag = 0; /* flag to indicate if label names valid */
    /* check if label name clashes with macro name */
    while (label_current != NULL) {
        macro_current = macro_head;
        while (macro_current != NULL) {
            /* trim trailing whitespace from the macro name  */
            trim_trailing_whitespace(macro_current->name);
            /* Search for a match */
            if (strcmp(label_current->labelName, macro_current->name) == 0) {
                printf("Line:%i ", label_current->am_line);
                print_error_msg(7);
                valid_flag = 1;
            }
            macro_current = macro_current->next;
        }
        /* check if label name clashes with reserved word; opname instruction or register */
        if (is_opname(label_current->labelName) || is_instruction(label_current->labelName)
            || is_reg(label_current->labelName)) {
            printf("Line:%i ", label_current->am_line);
            print_error_msg(9);
            valid_flag = 1;
        }
        label_current = label_current->next;
    }

    return valid_flag; /* 0 for success */
}

int duplicate_label_dcl(label *head) {
    label *to_compare;
    int dcl_flag;
    label *current;
    current = head;
    dcl_flag = 0; /* flag to indicate for duplicate declaration */
    while (current != NULL) {
        to_compare = current->next;
        while (to_compare != NULL) {
            if (strcmp(current->labelName, to_compare->labelName) == 0) {
                printf("line: %i line: %i\n", current->am_line, to_compare->am_line);
                print_error_msg(10);
                dcl_flag = 1; /* duplicate declaration was found */
            }
            to_compare = to_compare->next;
        }
        current = current->next;
    }
    return dcl_flag; /* 0 for success */
}

label* add_labels_to_list(FILE *file) {
   /* rewind(file);  */
    label *head;
    int line_num;
    char line[MAX_LINE_LENGTH];
    char name[MAX_LINE_LENGTH]; /* buffer for label name */
    char* decimal_address;
    char *colon_p;  /* pointer to the colon in the line */
    size_t len; /* length variable of the label name */
    char *extern_pos; /* pointer to the extern */
    head = NULL;
    line_num = 0;
    decimal_address = '\0'; /* not known yet, set to zero's */
    while (fgets(line, sizeof(line), file)) {
        line_num++;
        colon_p = strchr(line, ':'); /* locates the colon in the line */
        if (colon_p != NULL) {
             len = colon_p - line;
            strncpy(name, line, len);
            name[len] = '\0';
            /* labes that declaraed and defined in file will be created with temp unkown decimal address for now.
             * also extren and entry flags set to temp values*/
            add_label(&head, name,line_num,decimal_address,TEMP_VALUE,TEMP_VALUE);
        }
        /* seach for extern labes in file */
        extern_pos = strstr(line, ".extern");
        if (extern_pos != NULL) {
            extern_pos += strlen(".extern"); /* Move the pointer past ".extern" */
            /* Copy the label name after ".extern" until the end of the line */
            strncpy(name, extern_pos, MAX_LINE_LENGTH - 1);
            name[MAX_LINE_LENGTH - 1] = '\0';
            name[strcspn(name, "\n")] = '\0'; /* Remove any newline character */
            /* add to list with extern label decimal address */
                add_label(&head, name, line_num, ext_label_word(),TEMP_VALUE,TEMP_VALUE);
            }
        }
    /* check for entry and extern is here */
    rewind(file);
    while (fgets(line, sizeof(line), file)) {
        label *current = head;
        while (current != NULL) {
            if (strstr(line, current->labelName) != NULL) {
                if (strstr(line, ".entry") != NULL) {
                    current->IsEntry = ENTRY; /* entry found; set entry flag to 1; */
                } else if (strstr(line, ".extern") != NULL) {
                    current->IsExtern = EXTERN; /* extern found; set extern flag to 1 */
                }
            }
            current = current->next;
        }
    }
    return head;
}

int is_label(label *head, char *name) {
    label *current;
    current = head;
    while (current != NULL) {
        if (strcmp(current->labelName, name) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void release_labels(label *head) {
    label *current;
    label *next_label;
    current = head;
    while (current != NULL) {
        next_label = current->next;
        free(current->labelName);
        free(current);
        current = next_label;
    }
}


int is_label_declaration(char* line, char* name) {
    char *label_p;
    label_p = strstr(line, name);
    if (label_p != NULL) {

        char *after_label = label_p + strlen(name);
        if (*after_label == ':') {
            return 1;
        }
         return 0;

    }
    return 0;
}
/* debbuging only */
void print_labels(label *head) {
    label *current;
   current = head;
    while (current != NULL) {
        printf("Label: %s lable address:%s  FLagEnt:%i  FlagExt:%i\n" , current->labelName, current->dec_address,
            current->IsEntry, current->IsExtern);
        current = current->next;
    }
}


