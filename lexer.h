#ifndef LEXER_H
#define LEXER_H
#include "label_structs.h"
#include "data_struct.h"

/* count of opcodes for each addressing method*/
#define direct_opnames_count 5
#define label_opnames_count 14
#define dir_reg_opnames_count 11
#define indir_reg_opnames_count 14

/* structure for opcode */
typedef struct Opcode {
    char *opcode_name;
    int operand_amount;
} Opcode;

char* after_opcode(char *line, char *opcode,int line_num);
/**
 * @brief Searches for opcode in opcode array to find its index in the array
 *
 * The index will be convertd to its binary represntation
 *
 * @param name name of the opcode to search for.
 *
 * @return the index of the opcode in the opcodes array if found,
 *         otherwise -1.
 */
int search_opcode(char *name);

/**
 * @brief The function validates direct addressing for a given opcode and operand.
 *
 * This function checks if the operand used with a direct addressing method is valid.
 * and verifies that the operand is a valid integer
 *
 * @param opname A string representing the opcode name to check
 * @param operand A string representing the operand associated with the opcode.
 * @param line_number An integer representing the line number in am file.
 *
 * @return Returns 0 if the validation is successful.
 *         Returns 1 if the operand is not a valid integer or if the opcode does not support direct addressing.
 */
int valid_direct_addressing(char* opname,char* operand, int line_number);

/**
 * @brief The function Validates label addressing for a given opcode.
 *
 * This function checks whether the given opcode supports label addressing.
 *
 * @param opname A string representing the opcode name to check
 *
 * @return Returns 0 if the validation is successful.
 *         Returns 1 if the opcode does not support label addressing.
 */
int valid_label_addressing(char* opname, int line_number);

/**
 * @brief The function validates direct register addressing for a given opcode.
 *
 * This function checks whether the given opcode supports direct register addressing.
 *
 * @param opname A string representing the opcode name to check.
 * @param line_number An integer representing the line number in the am file
 *
 * @return Returns 0 if the validation is successful.
 *         Returns 1 if the opcode does not support direct register addressing.
 */
int valid_direct_reg_addressing(char* opname, int line_number);

/**
 * @brief The function validates indirect register addressing for a given opcode and operand.
 *
 * This function checks whether the given opcode supports indirect register addressing
 * and verifies that the operand is a valid register.
 *
 * @param opname A string representing the opcode name to check
 * @param operand A string representing the operand associated with the opcode
 * @param line_number An integer representing the line number in the am file
 *
 * @return Returns 0 if the validation is successful.
 *         Returns 1 if the operand is not a valid register or if the opcode does not support indirect reg addressing
 */
int valid_indirect_reg_addressing(char* opname,char* operand, int line_number);

/**
 * @brief The function validates that an opcode name which should have an empty operand does so.
 *
 * @param opname A string representing the opcode name to check
 * @param line_number An integer representing the line number in the am file
 *
 * @return Returns 0 if the opcode is "stop" or "rts"
 *         Otherwise, it returns 0.
 */
int valid_empty(char* opname, int line_number);

/**
* @brief The function checks each line of the file for syntax errors
*
* This function reads through each line of the provided file and checks if the line starts
* with a valid label, opcode, or directive.
* the function checks labes name; errors related to invalid label name will appear also
* in lines with label use.
* @param file - A pointer to the file to check
*
* @return Returns 1 if any syntax errors are found in the file, otherwise returns 0.
*/
int check_syntax_error(FILE *file);

/**
 * @brief Validates the correct placement and amount of quotes in .string line
 *
 * This function checks if the quotes  are correctly placed. It ensures that there are exactly
 * two quotes
 *
 * @param line The line of am file line with .string data line
 * @param line_num am file line number
 *
 * @return integer 1 for some error.  otherwise 0.
 */
int valid_string(char* line, int line_num);

char* handle_string(char* line);

int count_chars(char* line);

/**
 * @brief Validates the .data lines for correct format
 *
 * It ensures that the line is in correct format such as no leading or
 * trailing commas, and that only valid integer values (with optional plus or minus signs) are
 * present.
 *
 * @param line The line of am file line with .data data line
 * @param line_num am file line number
 *
 * @return integer 1 for some error.  otherwise 0.
 */
int valid_data(char* line, int line_num);

/**
 * @brief Counts the amount of integers in a data line

 * @param line The line of assembly code to be processed.
 *
 * @return The number of integers found in the line.
 */
int count_integers(char* line);

/**
 * @brief Converts integers in a ".data" line to binary and adds them to a linked list.
 *
 * This function converts each integer found into its binary representation, then adds the binary
 * string to a linked list of data
 *
 * @param head A double pointer to the head of the linked list where the binary data will be stored.
 * @param line The line of assembly code that contains .data
 */
void convert_add_integers(data_list **head, char* line);

/**
 * @brief The function converts each character between double quotes
 * an adds them to a linked list.
 *
* This function converts each char found into its binary representation, then adds the binary
 * string to a linked list of data
 * adds a binary string to the null terminator
 *
 * @param head A double pointer to the head of the linked list where the binary data will be stored.
 * @param line The line of assembly code that contains .string
 */
void convert_add_chars(data_list** head, char* line);
#endif
