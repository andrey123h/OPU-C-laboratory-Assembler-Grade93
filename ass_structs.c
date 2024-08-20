#include "ass_globals.h"
#include "ass_structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


machine_code* create_machine_code(first_word word, char* second_word, char* third_word){
    machine_code* new_machine_code;
    new_machine_code = malloc(sizeof(machine_code));
    new_machine_code->word = word;
    strcpy(new_machine_code->second_word, second_word);
    strcpy(new_machine_code->third_word, third_word);
    new_machine_code->next = NULL;
    return new_machine_code;
}

first_word* create_first_word(char* encode, char* dest_op,char* source_op, char*  opcode){
    first_word* new_first_word;
    new_first_word = malloc(sizeof(first_word));
    strncpy(new_first_word->encode, encode, ENCODING_SIZE);
    new_first_word->encode[ENCODING_SIZE] = '\0'; /* Ensure null termination */
    strncpy(new_first_word->dest_op, dest_op, OPERAND_SIZE);
    new_first_word->dest_op[OPERAND_SIZE] = '\0';
    strncpy(new_first_word->source_op, source_op, OPERAND_SIZE);
    new_first_word->source_op[OPERAND_SIZE] = '\0';
    strncpy(new_first_word->opcode, opcode, OPCODE_SIZE);
    new_first_word->opcode[OPCODE_SIZE] = '\0';
    new_first_word->next = NULL;
    return new_first_word;
}

void add_machine_code(machine_code** head, first_word word, char* second_word, char* third_word){
    machine_code* new_machine_code;
     new_machine_code = create_machine_code(word, second_word, third_word);
    if (*head == NULL) {
        *head = new_machine_code;
    } else {
        machine_code* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_machine_code;
    }
}

/* for debbuging only */
void print_machine_code_list(machine_code *head) {
    int index;
    machine_code *current = head;
    index = 0;

    while (current != NULL) {
        printf("Machine Code %d:\n", index++);
        printf("First Word: %s %s %s %s\n",
        current->word.opcode,
        current->word.source_op,
        current->word.dest_op,
        current->word.encode);
        printf("Second Word: %s\n", current->second_word);
        printf("Third Word: %s\n", current->third_word);
        printf("---------------------------\n");

        current = current->next;
    }
}

void free_machine_code_list(machine_code *head) {
    machine_code *current = head;
    machine_code *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}
