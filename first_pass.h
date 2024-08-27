#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#include "label_structs.h"
/* this is an inicador for second or third extra word that do no exict
 the word will be a binary rep of 15 bits of one's and will be deleted */
#define GARBAGE_VALUE '1'
#define ARE "100" /* ARE for first word */

typedef enum {
    IMMEDIATE_VALUE,
    LABEL,
    INDIRECT_REGISTER,
    DIRECT_REGISTER,
    EMPTY, /* stop or rts */
    INVALID_OPERAND
} OperandMethod;

typedef enum {
    OPERAND_SRC, /* source operand */
    OPERAND_DST /* dest operand */
} OperandSRC_DST;

/**
 * @brief This is the main function of the program.
 * The function converts assembly code to binary machine code
 * The function is using all the helper function to create the words by specific rules.
 * The machine code words then added to a list.
 *
 * @param file pointer to the assembly file
 * @param file_name name of the file, used for creating all the output files
 * @param head pointer to the head of a linked list of.use for validation of labales.
 *
 * @return Returns 0 if the first pass is successful and no errors are encountered. Returns 1 if any errors occur
 */
int exe_first_pass(FILE *file,char* file_name,macro* head);

/**
 * @brief Function to extract the operand following an opcode in a line of assembly code.
 * this will be the first operand, "source operand".
 * @param line The line of assembly code
 * @param opcode The opcode to search for within the line go past it
 * @param line_num The line number in the file
 *
 * @return pointer to the extracted operand string.
 */
char* after_opcode(char *line, char *opcode,int line_num);

/**
 * @brief Function to extract the operand following the comma in a line of assembly code.
 * this will be the secend operand, "dest/target operand".
 * @param line The line of assembly code
 * @param opcode The opcode to search for within the line go past it
 * @param line_num The line number in the file
 *
 * @return pointer to the extracted operand string.
 */
char* after_comma(char *line, char *opcode, int line_num);


/**
* @brief Function that Determines the addressing method of an operand.
*
* The function analyzes the provided operand to determine its addressing method
*
*  @param operand string of the operand to be checked
* @param label_head A pointer to the head of a linked list of labels. This list is used to check if the operand is label
*
* @return returns an OperandMethod enum value indicating the type of the operand
*/
OperandMethod what_operand_type(char *operand, label *label_head);

/**
* @brief Function that validates operand addressing method and converts it to binary representation.
* that will be used for its representation in the first word.
*
* @param type addressing method of operand, specified by the OperandMethod enum
* @param operand string of the operand
* @param opname The name of the opcode associated with the operand. for method validation
* @param line_num  line number of am file
* @return pointer to a dynamically allocated string representing the binary code of the operand
*  in the first word src/dst operand bits. NULL if its an illigal operand.
*/
char* handle_operand(OperandMethod type, char* operand, char* opname, int line_num);

/**
* @brief Function that converts operand to the extra word of the operand itself.
*
* The function skips certain characters to convert the values itself.
*
* @param type addressing method of operand, specified by the OperandMethod enum
* @param operand string of the operand
* @param src_or_dst enum value indicating if the operand is a source/dest operand
* @return pointer to a dynamically allocated string representing the binary code of the operand extra word
*
*/
char* operand_word(OperandMethod type, char *operand,OperandSRC_DST src_or_dst, label *list);

/**
* @brief Function that check the number of operands in a line of assembly code
* based on the amount of commas in line
*
*  @param line string of the line of assembly code to be analyzed
* @param opcode  string representing the opcode in the line to go past it.
*
* @return integer of the number of operands. -1 for more then 2 operands witch is illigal
*/
int check_operand_count(char *line, char *opcode);

/**
* @brief Function that checks if both operands are registers, based on their type.
*
* @param type1 enum value representing the method addressing type of the first operand.
* @param type2 enum value representing the method addressing type of the second operand.
*
* @return integer value 1 if both are registers, else will return 0.
*/
int is_reg_to_reg(OperandMethod type1, OperandMethod type2);

/**
* @brief Function that handles the conversion of register-to-register special extra word
*
* handles cases of register addressing for determine how many characters to skip
*
* @param src_reg The source register operand
* @param dst_reg The destination register operand
*
* @return pointer to a dynamically allocated string that combines the binary representation of both registers.
*/
char* handle_reg_to_reg(char* src_reg, char* dst_reg);

/**
 * @brief Checks if a given operand is an external label and, if so, adds it to the extern label list.
 *
 * If the operand matches a label that is flaged as external, the label and its address are added
 * to the extern label list.
 * this function is used for the .ext final file
 *
 * @param operand The operand to check if it is an external label.
 * @param head_ext double pointer to the head of the extern label linked list.
 * @param ic A pointer to the instruction counter
 * @param dc A pointer to the data counter.
 * @param head_label A pointer to the head of the label linked list
 */
void is_extern_label(char* operand, extern_label** head_ext,int *ic, int *dc, label* head_label);

#endif

