#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ass_globals.h"
#include "label_structs.h"
#include "file_handle.h"
#include "ass_structs.h"
#include "conversions.h"
#include "first_pass.h"
#include "errors.h"
#include "lexer.h"
#include "second_pass.h"
#include "data_struct.h"
#include "macr_struct.h"

extern Opcode opcodes[];

int exe_first_pass(FILE *file,char* file_name,macro* head_macro) {
    int valid_label_names, no_dup_dcl; /* for labes validation processes */
    extern_label* extern_head;
    machine_code *head_machine_code;
    label *labels_head;
    data_list *head_data;
    char *dest_bnr, *src_bnr, *secend_word, *third_word;
    first_word *word; /* first word of instruction */
    OperandMethod type1, type2; /* operand 1, operand 2 types */
    label *current; /* for label names iteration */
    int syntax_flag, func_error; /* erorr flags - 0 for success */
    char line[MAX_LINE_LENGTH];
    char* opcode_bnr; /* opcode in binary rep. */
    char* operand1, *operand2; /* first operand, second operand  */
    int operands_amount, operand_real_count, ch_count, int_count,word_counter; /* counters */
    int IC, DC, op_index;
    int line_num, i; /* line counter, for loop variable */
    line_num = 0;
    IC = MEMORY_START;
    DC = 0;
    func_error = 0;
    dest_bnr = NULL; /* dest operand in first word */
    src_bnr = NULL; /* source operand in first word */
    secend_word = NULL; /* extra info word one */
    third_word = NULL;  /* extra info word two */
    extern_head = NULL; /* list of extern labels */
    head_machine_code = NULL; /* machine code words list */
    rewind(file);
    labels_head = add_labels_to_list(file); /* list of labels in file */
    head_data = NULL; /* data linked list head */
    rewind(file);
    syntax_flag = check_syntax_error(file); /* check for syntax error*/
    valid_label_names = valid_label_name(labels_head,head_macro); /* check for clash with macro names */
    no_dup_dcl = duplicate_label_dcl(labels_head); /* check for duplicade labels */

     while (fgets(line, MAX_LINE_LENGTH, file) && IC + DC <= MAX_MEMORY_SIZE) {
        line_num++;
         if (strcmp(line, "\n") == 0) {
             continue;  /* ignore new lines */
         }
         word_counter = 0; /* reset word counter to zero */
         current = labels_head; /* reset current to labels head */

         /* ---- if its a label declaration ---- */
         for (; current != NULL; current = current->next) {
             /* if line contain label name and if the line is a label declartion */
             if (strstr(line, current->labelName) != NULL && is_label_declaration(line,current->labelName)) {
                 set_label_dec_address(labels_head, current->labelName, &IC, &DC);
             }
         }

         /* ---- if it's an .string data line ---- */
         if (strstr(line, ".string") != NULL) {
             if (valid_string(line,line_num) != 0) {
                 func_error = 1;
                 continue;
             }
             convert_add_chars(&head_data,line); /* convert to bnr and add to list */
             ch_count = count_chars(line); /* count nunmber of chars */
             DC += ch_count;
         }
         /* ---- if it's an .data data line ---- */
         if (strstr(line, ".data") != NULL) {
             if (valid_data(line,line_num) != 0) {
                 func_error = 1;
                 continue;
             }
             convert_add_integers(&head_data, line); /* convert to bnr and add to list */
             int_count = count_integers(line); /* count number of integers */
             DC += int_count;
         }
         /* ---- if it's an instruction line ---- */
        for (i = 0; i < OPCODES_COUNT; i++) {
            if (strstr(line, opcodes[i].opcode_name) != NULL) {
                /* convert opcode for the first word */
                op_index = search_opcode(opcodes[i].opcode_name);
                if (op_index != -1) {
                    opcode_bnr = bits_convertor(op_index, 4);
                    /* find the suitable number of operands, and real number of operand */
                    operands_amount = opcodes[i].operand_amount;
                    operand_real_count = check_operand_count(line, opcodes[i].opcode_name);
                    /* check if the number of operands is as expected to be */
                    if (operand_real_count != operands_amount) {
                        printf("line: %i", line_num);
                        print_error_msg(16);
                        func_error = 1;
                        continue;
                    }
                    /* ----two operands---- */
                    if (operands_amount == 2) {
                        /* get the operands */
                        operand1 = after_opcode(line, opcodes[i].opcode_name, line_num);
                        operand2 = after_comma(line, opcodes[i].opcode_name, line_num);
                        /* process operand1*/
                         type1 = what_operand_type(operand1, labels_head);
                        src_bnr = handle_operand(type1, operand1, opcodes[i].opcode_name, line_num);
                        if (src_bnr == NULL) {
                            func_error = 1;
                            continue;
                        }
                        secend_word = operand_word(type1,operand1,OPERAND_SRC,labels_head);
                        if (secend_word == NULL) {
                            func_error = 1;
                            continue;
                        }
                        /* process operand2 */
                        type2 = what_operand_type(operand2, labels_head);
                        dest_bnr = handle_operand(type2, operand2, opcodes[i].opcode_name, line_num);
                        if (dest_bnr == NULL) {
                            func_error = 1;
                            continue;
                        }
                        third_word = operand_word(type2,operand2,OPERAND_DST,labels_head);
                        if (third_word == NULL) {
                            func_error = 1;
                            continue;
                        }
                        /* ----if register to register----*/
                        if(is_reg_to_reg(type1,type2)) {
                            memset(secend_word, GARBAGE_VALUE, WORD_SIZE); /* no second word,set to garbage  */
                            secend_word = handle_reg_to_reg(operand1,operand2);
                            memset(secend_word, GARBAGE_VALUE, WORD_SIZE); /* no third word,set to garbage */
                            word_counter += operands_amount; /* one for first word, one for extra word; 2 word */
                            IC += word_counter;
                        }else {
                            word_counter += operands_amount + 1; /* three word */
                            IC += word_counter;
                        }
                        /* extern label handel, if operand is extern label */
                        is_extern_label(operand1,&extern_head,&IC,&DC,labels_head);
                        is_extern_label(operand2,&extern_head,&IC,&DC,labels_head);

                    }
                    /* ----One operand---- */
                     else if (operands_amount == 1) {
                        word_counter += operands_amount + 1; /* two words */
                         IC += word_counter;
                        operand1 = after_opcode(line, opcodes[i].opcode_name, line_num);
                        type1 = what_operand_type(operand1, labels_head);
                        dest_bnr = handle_operand(type1, operand1, opcodes[i].opcode_name, line_num);
                        if (src_bnr == NULL) {
                            func_error = 1;
                            continue;
                        }
                        secend_word = operand_word(type1,operand1,OPERAND_DST,labels_head);
                        if(secend_word == NULL) {
                            func_error = 1;
                            continue;
                        }
                        memset(third_word, GARBAGE_VALUE, WORD_SIZE); /* no third word */
                        /* set dest operand bits in first word to zero */
                        src_bnr = bits_convertor(0, OPERAND_SIZE);
                         /* extern label handel */
                        is_extern_label(operand1,&extern_head,&IC,&DC,labels_head);
                    }
                    /* ----no operands---- */
                     else if (operands_amount == 0) {
                        word_counter += operands_amount + 1; /* one word */
                         IC += word_counter;
                       /* process no empty operand */
                       /* set dest and src binary in first word to zero and set second and third word to garbage */
                        valid_empty(opcodes[i].opcode_name, line_num);
                        src_bnr = bits_convertor(0, OPERAND_SIZE);
                        dest_bnr = bits_convertor(0, OPERAND_SIZE);
                        memset(secend_word, GARBAGE_VALUE, WORD_SIZE); /* no second word */
                        memset(third_word, GARBAGE_VALUE, WORD_SIZE); /* no third word */
                    }


                     /* create the first word machine code. set ARE bits here becuse its always the same*/
                    word = create_first_word(ARE, dest_bnr, src_bnr, opcode_bnr);
                    add_machine_code(&head_machine_code, *word, secend_word, third_word);

                    free(word);
                    free(opcode_bnr);


                }
            }
        }
    }
    release_macro(head_macro); /* free the macro list, not needed anymore */
    if(syntax_flag == 0 && func_error == 0 && valid_label_names == 0 && no_dup_dcl == 0) {
        exe_second_pass(head_machine_code,labels_head); /* procced to second pass */
        /* procced creating the final files! */
        exe_final_files(file_name,labels_head,head_machine_code,extern_head,head_data,&IC,&DC);
        return 0;
    }
    return 1;
}

/* validation of operands and bnr convert. used for the first word */
char* handle_operand(OperandMethod type, char* operand, char* opname, int line_num) {
    char* bnr = NULL;
    int valid; /* validation flag. if validation fails will return null */
    switch (type) {
        case IMMEDIATE_VALUE:
            valid = valid_direct_addressing(opname, operand, line_num);
        if (valid != 0) {
            return NULL;
        }
        bnr = bits_convertor(DIRECT_ADDRESSING, 4);
        break;

        case LABEL:
            valid = valid_label_addressing(opname, line_num);
        if (valid != 0) {
            return NULL;
        }
        bnr = bits_convertor(LABEL_ADDRESSING, 4);
        break;

        case INDIRECT_REGISTER:
            valid = valid_indirect_reg_addressing(opname, operand, line_num);
        if (valid != 0) {
            return NULL;
        }
        bnr = bits_convertor(INDIRECT_REG_ADDRESSING, 4);
        break;

        case DIRECT_REGISTER:
            valid = valid_direct_reg_addressing(opname, line_num);
        if (valid != 0) {
            return NULL;
        }
        bnr = bits_convertor(DIRECT_REG_ADDRESSING, 4);
        break;

        case EMPTY:
            valid = valid_empty(opname, line_num);
        if (valid != 0) {
            return NULL;
        }
        bnr = bits_convertor(0, 4);
        break;

        case INVALID_OPERAND:
            default:
                printf("line: %i", line_num);
        print_error_msg(13);
        return NULL;
    }
    return bnr;
}

/* this is the extra word for each operand convertion to bnr rep */
char* operand_word(OperandMethod type, char *operand,OperandSRC_DST src_or_dst, label *list) {
    switch (type) {
        case IMMEDIATE_VALUE:
            /* Skip the '#' and convert */
            return value_to_word(bits_convertor(atoi(operand + 1), VALUE_SIZE));

        case LABEL:
            return operand; /* temporary value. the word of the label operand in the machine code will be its name.*/

        case INDIRECT_REGISTER:
            if (src_or_dst == OPERAND_SRC) { /* check if source or dest operand */
                return reg_to_word(bits_convertor(atoi(operand+2), REG_OP_SIZE), SRC_REG_START,
               SRC_REG_STOP); /* Skip the '*r' and convert */
            }
                return reg_to_word(bits_convertor(atoi(operand+2), REG_OP_SIZE), DST_REG_START,
                DST_REG_STOP);


        case DIRECT_REGISTER:
            if (src_or_dst == OPERAND_SRC) {
                return reg_to_word(bits_convertor(atoi(operand+1), REG_OP_SIZE), SRC_REG_START,
                SRC_REG_STOP); /* Skip the 'r' and convert */
            }
            return reg_to_word(bits_convertor(atoi(operand+1), REG_OP_SIZE), DST_REG_START,
                DST_REG_STOP);

        case EMPTY:
            return bits_convertor(0, WORD_SIZE);

        case INVALID_OPERAND:
        default:
           return NULL;
    }
}

/* chack what is the operand addressing method */
OperandMethod what_operand_type(char *operand, label *label_head) {
    if (strchr(operand, '#') != NULL) {
        return IMMEDIATE_VALUE;
    }
    if (is_label(label_head, operand)) { /* if operand is a known label from the label list */
        return LABEL;
    }
    if (strchr(operand, '*') != NULL) {
        return INDIRECT_REGISTER;
    }
    if (strchr(operand, '*') == NULL && is_reg(operand)) {
        return DIRECT_REGISTER;
    }
    if (operand == NULL || *operand == '\0') { /* for no operand */
        return EMPTY;
    }
    return INVALID_OPERAND;
}

/* function to go past the opcode and get the operand */
char* after_opcode(char *line, char *opcode,int line_num) {
    static char operand[32]; /* max label length */
    char *after_opcode; /* pointer to after the opcode in line */
    char *comma_pos; /* pointer to the comma in line */
    /* find the opcode in the line and set the pointer */
    after_opcode = strstr(line, opcode);
    if (after_opcode != NULL) {
        /* move past the opcode */
        after_opcode += strlen(opcode);
        /* locate the comma set pointer */
        comma_pos = strchr(after_opcode, ',');
        /* copy from after the opcode up to the comma into operand and ensure \0 */
        if (comma_pos != NULL) {
            strncpy(operand, after_opcode, comma_pos - after_opcode);
            operand[comma_pos - after_opcode] = '\0';
        } else {
            strcpy(operand, after_opcode);
            operand[strcspn(operand, "\n")] = '\0';
        }
        /*printf("operarnd 1: %s op: %s\n", operand, opcode); debugging only*/
    }
    trim_trailing_whitespace(operand); /* might have trailing whitespaces */
    return operand;
}

/* function to go past the comma and get the operand */
char* after_comma(char *line, char *opcode, int line_num) {
    static char operand[32]; /* max label length */
    char *after_opcode;
    char *comma_pos;
    after_opcode = strstr(line, opcode);
    if (after_opcode != NULL) {
        after_opcode += strlen(opcode);
        comma_pos = strchr(after_opcode, ',');
        if (comma_pos != NULL) {
            comma_pos++; /* move past the comma */
            strcpy(operand, comma_pos); /*Copy characters after the comma */
            operand[strcspn(operand, "\n")] = '\0'; /* remove newline character if present */
            /* printf("operand2: %s op: %s\n", operand, opcode); debugging only*/
        } else {
            operand[0] = '\0'; /*if no comma was found, set operand to an empty string */
        }
    }
    trim_trailing_whitespace(operand); /* might have trailing whitespaces */
    return operand;
}

int check_operand_count(char *line, char *opcode) {
    /* find position of the opcode in the line */
    char *after_opcode;
    int comma_count;
    char *comma_pos;
    after_opcode = strstr(line, opcode);
    comma_count = 0; /* comma counter */
    if (after_opcode != NULL) {
        after_opcode += strlen(opcode);
        trim_trailing_whitespace(after_opcode); /* might have trailing whitespaces */
        comma_pos = strchr(after_opcode, ',');
        while (comma_pos != NULL) {
            comma_count++;
            comma_pos = strchr(comma_pos + 1, ','); /* find  next comma */
        }
        if (comma_count > 1) {
            return -1; /* error more than one comma */
        }
        if (comma_count == 1) {
            return 2;/*  one comma is found */
        }
        /* no comma is found and there are characters after the opcode */
        if (*after_opcode != '\0') {
            return 1;
        }
        return 0; /* No operands was found */
    }
    return -1; /* error if got until this poit */
}

/* register to register functions */
int is_reg_to_reg(OperandMethod type1, OperandMethod type2) {
    if ((type1 == DIRECT_REGISTER || type1 == INDIRECT_REGISTER) &&
        (type2 == DIRECT_REGISTER || type2 == INDIRECT_REGISTER)) {
        return 1; /* both operands are registers */
        }
    return 0;
}

char* handle_reg_to_reg(char* src_reg, char* dst_reg){
    char* shared_word = NULL;
    char* src_bnr = NULL;
    char* dst_bnr = NULL;

    /* if indirect to indirect */
    if(strchr(src_reg, '*') != NULL && strchr(dst_reg, '*') != NULL) {
         src_bnr = bits_convertor(atoi(src_reg + 2), REG_OP_SIZE);
         dst_bnr = bits_convertor(atoi(dst_reg + 2), REG_OP_SIZE);
         shared_word = reg_reg_to_word(src_bnr, dst_bnr);
    }
    /* if direct to direct */
    if(strchr(src_reg, '*') == NULL && strchr(dst_reg, '*') == NULL) {
         src_bnr = bits_convertor(atoi(src_reg + 1), REG_OP_SIZE);
         dst_bnr = bits_convertor(atoi(dst_reg + 1), REG_OP_SIZE);
         shared_word = reg_reg_to_word(src_bnr, dst_bnr);
    }
    /* if indirect to direct */
    if(strchr(src_reg, '*') != NULL && strchr(dst_reg, '*') == NULL) {
         src_bnr = bits_convertor(atoi(src_reg + 2), REG_OP_SIZE);
         dst_bnr = bits_convertor(atoi(dst_reg + 1), REG_OP_SIZE);
         shared_word = reg_reg_to_word(src_bnr, dst_bnr);
    }
    /* if direct to indirect */
    if(strchr(src_reg, '*') == NULL && strchr(dst_reg, '*') != NULL) {
         src_bnr = bits_convertor(atoi(src_reg + 1), REG_OP_SIZE);
         dst_bnr = bits_convertor(atoi(dst_reg + 2), REG_OP_SIZE);
         shared_word = reg_reg_to_word(src_bnr, dst_bnr);
    }
    free(src_bnr); free(dst_bnr);
    return shared_word;
}

/* check if operand is extern label and adds to extern list */
void is_extern_label(char* operand, extern_label** head_ext,int *ic, int *dc, label* head_label) {
    label* current;
    int mention; /* decimal address where the extern was mentioned */
    current = head_label;
    mention = *ic + *dc -1 ; /* -1 for more readable code. the word counter is before invoking this func */
    while (current != NULL) {
        /* if operand is label name and is extern, add to extern label list */
        if (strcmp(current->labelName, operand) == 0) {
            if (current->IsExtern == 1) {
                add_extern_label(head_ext,operand,mention);
            }
        }
        current = current->next;
    }
}

