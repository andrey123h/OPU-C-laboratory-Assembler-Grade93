#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ass_globals.h"
#include "errors.h"
#include "conversions.h"

char* bits_convertor(int value, int size) {
    int i;
    char* binary_str;
    unsigned int unsigned_value;
    unsigned int mask;
    binary_str = new_word(size);
    i= 0;
    /* Handle negative values */
     unsigned_value = value < 0 ? (1 << size) + value : value;
    /* mask to extract each bit, starting from MSB */
    mask = 1 << (size - 1);
    /* Construct the binary string */
    for (; i < size; i++) {
        binary_str[i] = (unsigned_value & mask) ? '1' : '0';
        mask >>= 1;
    }
    binary_str[size] = '\0';
    return binary_str;
}

char* value_to_word(char *value_str) {
    int i;
    char *word;
    word = new_word(WORD_SIZE);
    i=0;
    /* Set bits 3-14 with the binary representation */
    for (; i < WORD_SIZE; i++) {
        word[i] = value_str[i];
    }
    set_absolute(word);
    return word;
}

char* reg_to_word(char *reg_str, int start, int stop) {
    int i;
    char *word;
    word = new_word(WORD_SIZE);
    i = 0;
    /* set all bits to '0' */
    memset(word, '0', WORD_SIZE);
    /* Set bits for source or dest register with the binary representation */
    for (; start <= stop; start++, i++) {
        word[start] = reg_str[i];
    }
    set_absolute(word);
    return word;
}

char* reg_reg_to_word(char *source_str,char *dest_str) {
    int i ;
    char *word;
    int source_start, source_stop,dest_start,dest_stop;
    source_start = SRC_REG_START;
    source_stop = SRC_REG_STOP;
     dest_start = DST_REG_START;
     dest_stop = DST_REG_STOP;
    word = new_word(WORD_SIZE);
    i = 0;
    /* set all bits to '0' */
    memset(word, '0', WORD_SIZE);

    /* Set bits of source register with the binary representation */
    for (; source_start <= source_stop; source_start++, i++) {
        word[source_start] = source_str[i];
    }
    i = 0;
    /* Set bits of dest register with the binary representation */
    for (; dest_start <= dest_stop; dest_start++, i++) {
        word[dest_start] = dest_str[i];
    }
    set_absolute(word);
    return word;
}

char* ext_label_word () {
    char* word;
    word = new_word(WORD_SIZE);
    memset(word, '0', WORD_SIZE);
    set_external(word);
    return word;

}

char* rel_label_word(char *address_str) {
    int i;
    char* word;
     word = new_word(WORD_SIZE);
    /* Set bits 3-14 with the binary representation */
    i = 0;
    for (; i < LABEL_SIZE; i++) {
        word[i] = address_str[i];
    }
    set_relocatable(word);
    return word;
}

char* new_word(int size) {
    char *new_word;
    /* Allocate memory for the word string */
    new_word = malloc(size + 1);
    if (new_word == NULL) {
        print_error_msg(5);
        return NULL;
    }
    new_word[size] = '\0';
    return new_word;
}

void set_absolute(char *word) {
    word[12] = '1';
    word[13] = '0';
    word[14] = '0';
}

void set_relocatable(char *word) {
    word[12] = '0';
    word[13] = '1';
    word[14] = '0';
}

void set_external(char *word) {
    word[12] = '0';
    word[13] = '0';
    word[14] = '1';
}

/* convert strings data */
char* char_to_word(char ch) {
    return bits_convertor(ch, CHAR_SIZE);
}

/* second pass converters */

char* convert_binary_to_octal(char* binary_str) {
    int decimal_value;
    char* octal_str; /* buffer for octal the octal string */
    /* convert binary string to int */
    decimal_value = strtol(binary_str, NULL, 2);

   /* allocate memory to octal string including null terminator */
    octal_str = (char*)malloc(6 * sizeof(char));

    if (octal_str == NULL) {
        print_error_msg(5);
        return NULL;
    }
    sprintf(octal_str, "%05o", decimal_value);
    return octal_str;
}

/* to convert leabel addres */
int binary_to_decimal (char* binary_str) {
    int decimal_value;
    decimal_value = strtol(binary_str, NULL, 2);
    return decimal_value;
}

/* to extract the labels address */
char* extract_12_bits(char* binary_str) {
    char* ext_bits; /* buffer to store the extracted 12 bits without ARE */
    ext_bits = (char*)malloc(13 * sizeof(char)); /* 12 bits + 1 for null terminator */

    if (ext_bits == NULL) {
        print_error_msg(5);
        return NULL;
    }

    /* copy the first 12 bits from the binary string */
    strncpy(ext_bits, binary_str, 12);
    ext_bits[12] = '\0';
    return ext_bits;
}