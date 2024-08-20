#ifndef MACR_STRUCT_H
#define MACR_STRUCT_H

/* Represents a node; macro in a linked list */
typedef struct macro {
    char *name;
    char *body; /* contents of the macro */
    struct macro *next;
} macro;

/**
 * @brief Creates a new macro node
 *
 * Creates a new macro with the given name and body
 * and initializes its fields
 *
 * @param name a string of the name of the macro
 * @param body the content of the macro
 *
 * @return a new node of a macro
 */
macro* create_macro(char *name, char *body);

/**
 * @brief Adds a node to a linked list of macros
 *
 * This function adds a new node of a macro to the end of a linked list
 *
 * @param head a pointer to head of a linked list
 * @param name a string with the name of the new macro
 * @param body  a string with the content of the new macro
 */
void add_macro(macro **head, char *name, char *body);

/**
 * @brief release the memory allocated for linked list
 * @param head a pointer to head of a linked list
 */
void release_macro(macro *head);

/**
 * @brief Searches for a node in a linked list by its name.
 *
 * This function searchs for a macro name in a linked list
 *
 * @param head a pointer to head of a linked list
 * @param name the string with the name of the macro to search
 * @param found  variable that indicates if the macro was found or not
 * @return the node of the macro if it was found, NULL if not
 */
macro* search_macro(macro *head, char *name, int *found);

/* for debugging only */
void display_macros(macro *head);

#endif