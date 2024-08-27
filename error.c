#include <stdio.h>
#include "errors.h"

void print_error_msg(int error_code) {

    switch(error_code) {
        case 1:
            /* Print the error code number and the error message */
            printf("**ERROR** CODE:%d | %s\n", error_code,"endmacr has additional char");
        break;

        case 2:
            printf("**ERROR** CODE:%d | %s\n", error_code,"Illegal macro name");
        break;

        case 3:
            printf("**ERROR** CODE:%d | %s\n", error_code,"Failed to open file");
        break;

        case 4:
            printf("**ERROR** CODE:%d | %s\n", error_code,"Illegal characters before macr");
        break;

        case 5:
            printf("**ERROR** CODE:%d | %s\n", error_code,"Failed to dynamically allocate memory");
        break;

        case 6:
            printf("**ERROR** CODE:%d | %s\n", error_code,"Illegal macro declaration; must have name and body");
        break;

        case 7:
            printf("**ERROR** CODE:%d | %s\n", error_code,"label match macro name");
        break;

        case 8:
            printf("**ERROR** CODE:%d | %s\n", error_code,"macro already exist; not added to list");
        break;

        case 9:
            printf("**ERROR** CODE:%d | %s\n", error_code,"Illegal label name");
        break;

        case 10:
            printf("**ERROR** CODE:%d | %s\n", error_code,"Illegal label declaration; duplicate declaration");
        break;

        case 11:
            printf("**ERROR** CODE:%d | %s\n", error_code,"operation and operand are not suitable");
        break;

        case 12:
            printf("**ERROR** CODE:%d | %s\n", error_code,"Illegal register");
        break;

        case 13:
            printf("**ERROR** CODE:%d | %s\n", error_code,"Illegal operand or unkown operand");
        break;

        case 14:
            printf("**ERROR** CODE:%d | %s\n", error_code,"operand following '#' is non-digit");
        break;

        case 15:
            printf("**ERROR** CODE:%d | %s\n", error_code,"'#' without following integer");
        break;

        case 16:
            printf("**ERROR** CODE:%d | %s\n", error_code,"number of operands are not as expected");
        break;

        case 17:
            printf("**ERROR** CODE:%d | %s\n", error_code,"Illegal label name; alphanumeric only.");
        break;

        case 18:
            printf("**ERROR** CODE:%d | %s\n", error_code,"line must start with label/opcode/directive");
        break;

        case 19:
            printf("**ERROR** CODE:%d | %s\n", error_code,"Illegal string line syntax");
        break;

        case 20:
            printf("**ERROR** CODE:%d | %s\n", error_code,"Illegal data line syntax");
        break;

        case 21:
            printf("**ERROR** CODE:%d | %s\n", error_code,"faield to create output file");
        break;


        default:
            printf("**ERROR** CODE:%d | %s\n", error_code, "unknown error ");
        break;
    }
}




