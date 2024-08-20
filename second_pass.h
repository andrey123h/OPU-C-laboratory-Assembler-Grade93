#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include "label_structs.h"
#include "ass_structs.h"
#include "data_struct.h"


#define IGNORE "111111111111111" /* this is a bnr rep string for second or third word to ignore */

/**
 * @brief The function is sets the decimal address of a label
 *
 * the funcion is calculates the decimal address
 * The calculated address is converted to a binary string
 *
 * @param head Pointer to the head of the label linked list.
 * @param name The name of the label whose address is to be set.
 * @param ic Pointer to the instruction counter
 * @param dc Pointer to the data counter
 */
void set_label_dec_address(label *head,char* name, int *ic, int *dc);
/* debbuing only */
char* get_label_word(label *head, char* name);

/**
 * @brief Executes the second pass of the assembler, replacing label names in the machine code with their addresses.
 *
 * This function iterates through the machine code list and checks each machine code's second_word and third_word
 * If these fields contain a label name, the function replaces them with the corresponding label's binary address.
 *
 * @param head_machine Pointer to the head of the machine code linked list.
 * @param head_label Pointer to the head of the label linked list.
 */
void exe_second_pass (machine_code* head_machine,label* head_label);


/**
 * @brief Generates the final output files for the assembly process.
 *
 * This function creates three different types of output files:
 * 1. The object file (.ob) that contains the machine code and data in octal format.
 * 2. The entry file (.ent) that contains entry labels if exisct and their addresses.
 * 3. The extern file (.ext) that contains extern labels if exisct and their mention addresses.
 *
 * @param file_name The name of the input file, which will be used to create output files with different extensions.
 * @param label_head The head of the linked list of labels. .
 * @param machine_head The head of the linked list of machine code instructions (words)
 * @param extern_head The head of the linked list of extern labels.
 * @param data_head The head of the linked list of data.
 * @param ic A pointer to the instruction counter
 * @param dc A pointer to the data counter
 */
void exe_final_files(char* file_name, label* label_head, machine_code* machine_head,extern_label* extern_head,
                   data_list* data_head, int* ic, int* dc);

#endif