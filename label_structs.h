#ifndef LABEL_STRUCTS_H
#define LABEL_STRUCTS_H
#include <stdio.h>
#include "macr_struct.h"

/* if the label is not entery and not extern the values will remain -1 */
#define TEMP_VALUE -1 /* temp value for flags before entry and extren check */
#define EXTERN 1
#define ENTRY 1

/* Represents a node; label in a linked list */
typedef struct label {
    char *labelName;
    int am_line; /* line in the am file */
    char* dec_address; /* decimal address in binary represantion */
    int IsExtern; /* extern flag; 1 for extern, -1 if not */
    int IsEntry; /*  entry flag; 1 for entry, -1 if not */
    int dec_add_of_extern; /* decimal address for extern label. for the ext file */
    struct label *next;
} label;

/* Represents a node; extern label in a extern labels linked list */
typedef struct extern_label {
    char *externName;
    int extern_mention; /* decimal adrress of extern label mention */
    struct extern_label *next;
} extern_label;

/**
 * @brief Creates a new label node of a label struct
 *
 * The function allocates memory for a new label node
 * The label name is dynamically allocated
 *
 * @param name a string of the name of the label
 * @param am_line_num a integer of the line number in the am file
 * @param dec_address dicimal address of the label.
 * @param is_extern an integer flag to indicate if labal is extern
 * @param is_entry an integer flag to indicate if labal is entry
 * @return pointer to a new node of label. NULL if memmory allocation failed.
 */
label* create_label(char *name, int am_line_num, char* dec_address,  int
    is_extern,int is_entry);

/**
 * @brief Adds a node to a linked list of label structs.
 * This function adds a new node of a label to the end of a linked list
 *
 * @param head a pointer to head of a linked list
 * @param name a string to the name of the label
 * @param am_line_num a integer of the line number in the am file
*  @param dec_address dicimal address of the label.
 * @param is_extern an integer flag to indicate if labal is extern
 * @param is_entry an integer flag to indicate if labal is entry
 *
 */
void add_label(label **head, char *name,int am_line_num,char* dec_address, int
    is_extern, int is_entry);

/**
 * @brief Cheks if label names are valid
 *
 * This function checks if each label name matches any name in the macro linked list
 * or if the label name is a reserved word.
 *
 * @param label_head A pointer to the  head of the label linked list.
 * @param macro_head A pointer to the head of the macro linked list.
 * @return returns integer. 1 if any invalid label names are found, otherwise 0.
 */
int valid_label_name(label *label_head, macro *macro_head);

/**
 * @brief Function to add labels nodes to labels linked list, and sets extern and entry
 * The function extract labels and add them to the list
 * This function searches for label identified by the presence of a colon
 * And labels declared with `.extern`
 * If a label is found, it is added to linked list
 *
 * At this point the decimal address not known yet
 * only extern label decimal address is set
 *
 * The function olso checks for extern and entry labels.
 * if found sets the extern and entry flags in the corresponding labels.
 *
 * @param file A pointer file.
 * @return A pointer to the head of the labels linked list
 */
label* add_labels_to_list(FILE *file);

/**
 * @brief Function to check duplicate label names in the linked list.
 *
 * This function iterates through the labesl linked list and checks for duplicate
 * label names.
 *
 * @param head A pointer to the head of the label linked list.
 * @return Returns integer 1 if duplicates are found, otherwise 0.
 */
int duplicate_label_dcl(label *head);

/**
 * @brief Checks if a given label name exists in the linked list of labels.
 *
 * This function searches through a linked list of labels to determine if a label
 * with the specified name exists.
 *
 * @param head A pointer to the head of the linked list of labels.
 * @param name A string representing the name of the label to search for.
 *
 * @return Returns 1 if the label is found in the list.
 *         Returns 0 if the label is not found.
 */
int is_label(label *head, char *name);

/**
 * @brief release the memory allocated for linked list
 * @param head a pointer to head of a linked list
 */
void release_labels(label *head);

/**
 * @brief Checks if a given line contains a label declaration.
 *
 * This function determines if a given label name is declared in the provided line.
 * declaration is identified by the presence of the label name followed by a colon
 *
 * @param line The line of text to be checked for the label declaration.
 * @param name The name of the label to search for in the line.
 *
 * @return `1` if the label is declared in the line, `0` otherwise.
 */
int is_label_declaration(char* line, char* name);

/* for debugging only */
void print_labels(label *head);

/* extern label function */

extern_label* create_extern_label(char *name, int mention);
void add_extern_label(extern_label **head, char *name, int mention);
void release_extern_labels(extern_label *head);
void print_extern_labels(extern_label *head);

#endif
