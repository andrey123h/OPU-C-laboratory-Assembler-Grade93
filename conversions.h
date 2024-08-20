#ifndef CONVERSIONS_H
#define CONVERSIONS_H
/**
* @brief Function to convert a value to a binary string representation.
 *
 * The funtion converts a value to a specified size of bits
 *
 * @param value value to be converted
 * @param size size of the binary string
 *
 * @return pointer to dynamically allocated string with the binary representation.
 * or NULL if memory allocation fails
 */
char* bits_convertor(int value, int size);

/**
 * @brief Function to construct value operand to a word (machine code)
 *
 * the function sets the 12 bits binary representation of an integer
 * and sets the ARE fields.
 *
 * @param value_str string of binary representation of an integer
 * @return A pointer to a dynamically allocated string containing the word representation.
 * or NULL if memory allocation fails
 */
char* value_to_word(char *value_str);

/**
 * @brief Function to construct register operand to a word (machine code)
 *
 * the function sets the 3 bits binary representation of register number
 * and sets the ARE fields.
 *
 * @param reg_str string of binary representation of register number
 * @param start the start of the source or dest register operand in a word.
 * @param stop the end of the source or dest register operand in a word.
 * @return A pointer to a dynamically allocated string containing the word representation.
 * or NULL if memory allocation fails
 */
char* reg_to_word(char *reg_str, int start, int stop);

/**
 * @brief Function to construct register to register operands to a word (machine code)
 *
 * the function sets the 3 bits binary representation of source register
 * the function sets the 3 bits binary representation of destenation register
 * and sets the ARE field.
 *
 * @param source_str string of binary representation of source register
 * @param dest_str string of binary representation of destenation register
 *
 * @return A pointer to a dynamically allocated string containing the word representation.
 * or NULL if memory allocation fails
 */
char* reg_reg_to_word(char *source_str,char *dest_str);

/* function to construct extren label word */
char* ext_label_word();
/* function to construct relocatable label word */
char* rel_label_word(char *address_str);

/* ARE */
void set_absolute(char *word); /* turns on A field */
void set_relocatable(char *word); /* turns on R field */
void set_external(char *word); /* turns on E field */

/**
 * @brief Allocates memory for a new word (machine code)
 *
 * This function allocates memory for a new word with specified size
 *
 * @param size The size of the word to be allocated
 * @return A pointer to the new allocated word.
 * Returns NULL if memory allocation fails.
 */
char* new_word(int size);

char* char_to_word(char ch);

char* convert_binary_to_octal(char* binary_str);

int binary_to_decimal (char* binary_str);
char* extract_12_bits(char* binary_str);

#endif