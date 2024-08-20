#ifndef PREPROCESS_H
#define PREPROCESS_H


/**
 * @brief Checks if the endmacr does not contain any additional character
 *
 * cheks both leading and trailing extra characters.
 *
 * @param file pointer to a text file.
 *
 * @return Returns an integer. if endmacr contain any additional character
 *  it returns 1. Otherwise, it returns 0.
 *
 */
int valid_endmacr(FILE *file);

/**
 * @brief Checks if the macr name does not contain any illegal characters.
 *
 * cheks for characters from a know list.
 *
 * @param file pointer to a text file.
 *
 * @return Returns an integer.if  macr name contain any illegal characters,
 *  it returns 1. Otherwise, it returns 0.
 *
 */
int valid_macro_names(FILE *file);


/* execute the checks for valid macro declarations */
int exe_validate_declarations(FILE *file);

/**
 * @brief This function adds a macro to the linked list.
 *
 * adding to linked list both name and body
 * additional chek for not empty macro name, and not empty macro body
 * adds the macro to the linked list if its not exist aleary
 * if exicst; only error msg will be printed .
 *
 * @param file pointer to a text file.
 * @return pointer to linked list head. NULL if not illigal declaration
 */
macro* add_macro_to_list(FILE *file);

/**
 * @brief This function removes all macros declarations from file
 *
 * @param file pointer to a file name.
 * removes the start of the declaration, the body, and the stop of declaration
 * Also cheks for comment line starting with ";", and removes Those lines.
 * creates new file for output with ".preproc" extension
 *
 * @return pointer to new file name
 */

char* declaration_removal(char *file);

/**
 * @brief This function replace macro names with their bodies
 *
 * @param file pointer to a file name.
 * @param head pointer to the linked list of macros
 *
 * creates new file for output with ".am" extension
 * This wiil be the  finel file after preprocees
 *
 * @return pointer to new file name
 */

char* replace_names_with_body(char *file, macro *head);

#endif