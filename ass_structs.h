#ifndef ASS_STRUCTS_H
#define ASS_STRUCTS_H
#include"ass_globals.h"

/* structs */
/* the +1 for null termintation */
typedef struct first_word {
    char opcode[OPCODE_SIZE + 1]; /* opcoude field */
    char source_op[OPERAND_SIZE + 1]; /* src operand */
    char dest_op[OPERAND_SIZE + 1]; /* dest operand */
    char encode[ENCODING_SIZE + 1]; /* ARE field */
    struct first_word *next;
} first_word;

/* represents a node of machine code in linked list */
typedef struct machine_code {
    first_word word;
    char second_word[WORD_SIZE + 1];
    char third_word[WORD_SIZE +1];
    struct machine_code *next;
} machine_code;

/* functions */

/**
 * @brief Creates a new machine code node
 *
 * This function allocates memory for a new machinde code node,
 *
 * @param word The first word structure representing the first word of the machine code.
 * @param second_word A string representing the second word of the machine code. (word for operand)
 * @param third_word A string representing the third word of the machine code. (word for operand)
 *
 * @return A pointer new machine code struct. If memory allocation fails,
 *         the function returns NULL
 */
machine_code* create_machine_code(first_word word, char* second_word, char* third_word);

/**
 * @brief Adds a new machine code node to the end of the linked list.
 *
 * This function creates a new machine code node  and adds the node to the end of the linked list.
 *
 * @param head A double pointer to the head of the linked list.
 * @param word The  first word structure representing the first word of the machine code.
 * @param second_word A string representing the second word of the machine code.
 * @param third_word A string representing the third word of the machine code.
 */
void add_machine_code(machine_code** head, first_word word, char* second_word, char* third_word);

/**
 * @brief The function create first word structure and initializes its fields.
 *
 * This function allocates memory for a new `first_word` structure, initializes its fields
 * with the provided `encode`, `dest_op`, `source_op`, and `opcode` values, and returns
 * a pointer to the newly created structure.
 *
 * @param encode A string representing the encoding field of the first word.
 * @param dest_op A string representing the destination operand field of the first word.
 * @param source_op A string representing the source operand field of the first word.
 * @param opcode A string representing the opcode field of the first word.
 *
 * @return A pointer to the new first word struct. If memory allocation fails,
 *         the function returns NULL
 */
first_word* create_first_word(char* encode, char* dest_op,char* source_op, char*  opcode);

/**
 * @brief Frees the memory allocated for a linked list of machine code nodes.
 *
 * @param head A pointer to the head of the linked list
 */
void free_machine_code_list(machine_code *head);

/* for debbuging only */
void print_machine_code_list(machine_code *head);
#endif



