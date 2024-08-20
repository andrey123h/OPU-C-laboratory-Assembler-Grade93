#ifndef ASS_GLOBALS_H
#define ASS_GLOBALS_H

#define MAX_LINE_LENGTH 81

#define MAX_MEMORY_SIZE 4096
#define MEMORY_START 100

/* bits sizes */
#define WORD_SIZE 15
#define ENCODING_SIZE 3
#define OPCODE_SIZE 4
#define OPERAND_SIZE 4
#define VALUE_SIZE 12
#define REG_OP_SIZE 3
#define LABEL_SIZE 12
#define CHAR_SIZE 15

/* reg bits in first word */
#define SRC_REG_START 6
#define SRC_REG_STOP 8
#define DST_REG_START 9
#define DST_REG_STOP 11

/* counts */
#define INSTRUCTIONS_COUNT 4
#define REG_COUNT 8
#define OPCODES_COUNT 16

/* addressing methods to convert */
#define DIRECT_ADDRESSING 1
#define DIRECT_REG_ADDRESSING 8
#define INDIRECT_REG_ADDRESSING 4
#define LABEL_ADDRESSING 2

#define MAX_FILE_LENGTH 100 /* assume max 100 charaters for file name */


#endif
