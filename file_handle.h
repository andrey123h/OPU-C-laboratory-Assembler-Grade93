#ifndef FILE_HANDLE_H
#define FILE_HANDLE_H
#define INSTRUCTIONS_COUNT_CHEK 4
#define REG_COUNT_CHEK 8
#define INDIRECT_REGS_COUNT_CHECK 8
#define OPCODES_COUNT_CHEK 8

/**
 * @brief Creates a new file name by appending an extension to the original file name.
 *
 * This function allocates memory for a new file name, copies the original name
 * up to the first '.' character (if it exists), and appends the specified
 * extension to the new file name.
 *
 * @param name The original file name.
 * @param extension The extension to append to the new file name.
 * @return pointer to the new file name
 */

char *new_file(char *name, char *extension);

/**
 * @brief Function to remove whitespaces from the input file and write to a new file
 *
 * the function writes each non-whitespace character to a new file.
 * creates new file for output with ".firstproc" extension
 *
 * @param input_filename The original file name.
 * @return pointer to new file name
 */

char *remove_whitespaces(char *input_filename);


void trim_trailing_whitespace(char *str);

/**
 * @brief Functions checks if the provided string is  opname
 *
 * If a match is found, the function will return 1.
 * If no match is found, it returns 0.
 *
 * @param str pointer to string to check.
 *
 * @return Returns an integer. If the string matches one of the known characters,
 *  it returns 1. Otherwise, it returns 0.
 */

int is_opname(char *str);
/**
 * @brief Functions checks if the provided string is register
 *
 * If a match is found, the function will return 1.
 * If no match is found, it returns 0.
 *
 * @param str pointer to string to check.
 *
 * @return Returns an integer. If the string matches one of the known characters,
 *  it returns 1. Otherwise, it returns 0.
 */

int is_reg(char *str);

/**
 * @brief Functions checks if the provided string is instruction.
 *
 * If a match is found, the function will return 1.
 * If no match is found, it returns 0.
 *
 * @param str pointer to string to check.
 *
 * @return Returns an integer. If the string matches one of the known characters,
 *  it returns 1. Otherwise, it returns 0.
 */

int is_instruction(char *str);

int is_indirect_reg(char *str);

/* function to copy file contents to new file */
void copy_file(char *name_of_dest, char *name_of_src);

#endif
