#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ass_globals.h"
#include "label_structs.h"
#include "second_pass.h"
#include "file_handle.h"
#include "ass_structs.h"
#include "conversions.h"
#include "errors.h"
#include "data_struct.h"

/* set the labes decimal address in the label list  */
void set_label_dec_address(label *head,char* name, int* ic, int* dc) {
    int decimal_address;
    char* address_with_are;
    char* converted_address;
    label *current;
    current = head;
    decimal_address = *ic + *dc; /* not converted to binary decimal address */
    converted_address = bits_convertor(decimal_address,LABEL_SIZE);
    /* converted decimal address and set to rel ARE */
     address_with_are = rel_label_word(converted_address);
    while (current != NULL) {
        if (strcmp(current->labelName, name) == 0){
            current-> dec_address = address_with_are;
        }
        current = current->next;
    }
}

/* get the given lables its decimal address */
char* get_label_word(label *head, char* name) {
    label *current;
    current = head;
    while (current != NULL) {
        if (strcmp(current->labelName, name) == 0){
           return current->dec_address;
        }
        current = current->next;
    }
    return NULL;
}

/* replacment of label operand names with its actual addrees for label extra word */
void exe_second_pass (machine_code* head_machine,label* head_label) {
        label *label_current;
        machine_code *code_current;
        code_current = head_machine;

        while (code_current != NULL) {
            label_current = head_label;
            while (label_current != NULL) {

                if (strcmp(code_current->second_word, label_current->labelName) == 0) {
                    strncpy(code_current->second_word, label_current->dec_address, WORD_SIZE);
                    code_current->second_word[WORD_SIZE] = '\0';
                }

                if (strcmp(code_current->third_word, label_current->labelName) == 0) {
                    strncpy(code_current->third_word, label_current->dec_address, WORD_SIZE);
                    code_current->third_word[WORD_SIZE] = '\0';
                }

                label_current = label_current->next;
            }
            code_current = code_current->next;
        }
    /* print_machine_code_list(head_machine); */
    /* print_labels( head_label); */

}


void exe_final_files(char* file_name, label* label_head, machine_code* machine_head,extern_label* extern_head,
                   data_list* data_head, int* ic, int* dc) {
    FILE *ob_filep;
    char *ob_file_name;
    int address; /* for decimal address prints */
    char first_word[WORD_SIZE + 1];
    char* exct_dec; /* for the extraction of label 12 bits address without ARE */
    char *octal_first_word, *octal_second_word, *octal_third_word, *octal_data; /*pointers for octal strings */
    int converted_dec;

    char *ext_file_name, *ent_file_name;
    FILE *ext_filep = NULL;
    FILE* ent_filep = NULL;
    machine_code* current_word;
    data_list* current_data;
    label* current_label;
    extern_label* current_extern;
    address = MEMORY_START;



    ob_file_name = new_file(file_name, ".ob");
    ob_filep = fopen(ob_file_name, "w");
    if (ob_filep == NULL) {
        print_error_msg(21);
        free(ob_file_name);
        return;
    }
    /* print IC and DC to ob file */
    fprintf(ob_filep, "    %d %d\n", *ic - MEMORY_START, *dc);

    /* print the machine code list to ob file */
    /* format the first word of instruction into a one binary string */
    current_word = machine_head;
    while (current_word != NULL) {
        sprintf(first_word, "%s%s%s%s",
                 current_word->word.opcode,
                 current_word->word.source_op,
                 current_word->word.dest_op,
                 current_word->word.encode);

        octal_first_word = convert_binary_to_octal(first_word);
        fprintf(ob_filep, "%04d     %s\n", address++, octal_first_word);
        free(octal_first_word);

         /* convert and print second_word if it's not IGNORE - "111111111111111"*/
        if (strncmp(current_word->second_word, IGNORE,WORD_SIZE) != 0) {
            octal_second_word = convert_binary_to_octal(current_word->second_word);
            fprintf(ob_filep, "%04d     %s\n", address++, octal_second_word);
            free(octal_second_word);
        }

        /* convert and print second_word if it's not IGNORE */
        if (strncmp(current_word->third_word, IGNORE,WORD_SIZE) != 0) {
            octal_third_word = convert_binary_to_octal(current_word->third_word);
            fprintf(ob_filep, "%04d     %s\n", address++, octal_third_word);
            free(octal_third_word);
        }

        current_word = current_word->next;
    }

    /* convert and print the data list to ob file */
    current_data = data_head;
    while (current_data != NULL) {
        octal_data = convert_binary_to_octal(current_data->data_bnr);
        fprintf(ob_filep, "%04d     %s\n", address++, octal_data);
        free(octal_data);

        current_data = current_data->next;
    }

    /* create and print to the entry file if entry labels exist */
    current_label = label_head;
    while (current_label != NULL) {

        if (current_label->IsEntry == 1) {
            if (ent_filep == NULL) {
                ent_file_name = new_file(file_name, ".ent");
                ent_filep = fopen(ent_file_name, "w");

                if (ent_filep == NULL) {
                    print_error_msg(21);
                    free(ent_file_name);
                    break;
                }
            }
            /* extract the 12 bits for from labes address convert it to a decimal value */
            exct_dec = extract_12_bits(current_label->dec_address);
            converted_dec = binary_to_decimal(exct_dec);
            fprintf(ent_filep, "%s %i\n", current_label->labelName, converted_dec);
            free(exct_dec);
        }

        current_label = current_label->next;
    }

    /* create and print to the extern (.ext) file if extern labels exist */
    if (extern_head != NULL) {
        ext_file_name = new_file(file_name, ".ext");
        ext_filep = fopen(ext_file_name, "w");

        if (ext_filep == NULL) {
            print_error_msg(21);
            free(ext_file_name);
            return;
        }

        current_extern = extern_head;
        while (current_extern != NULL) {
            fprintf(ext_filep, "%s %04d\n", current_extern->externName, current_extern->extern_mention);
            current_extern = current_extern->next;
        }

        fclose(ext_filep);
        free(ext_file_name);
    }

    fclose(ob_filep);
    free(ob_file_name);

    if (ent_filep != NULL) {
        fclose(ent_filep);
        free(ent_file_name);
    }

    /* free memory for all lists that were used*/
    free_machine_code_list(machine_head);
    release_labels(label_head);
    release_extern_labels(extern_head);
}

