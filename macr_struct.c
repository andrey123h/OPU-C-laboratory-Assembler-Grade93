#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macr_struct.h"
#include "errors.h"

macro* create_macro(char *name, char *body) {
    /* Allocate memory for the new macro structure, name and body */
    macro *new_macro;
    new_macro = malloc(sizeof(macro));
    new_macro->name = malloc(strlen(name) + 1);
    strcpy(new_macro->name, name);
    new_macro->body = malloc(strlen(body) + 1);
    strcpy(new_macro->body, body);
    new_macro->next = NULL;
    if (new_macro->name == NULL || new_macro->body == NULL) {
        print_error_msg(5);
        return NULL;
    }
    return new_macro;
}

void add_macro(macro **head, char *name, char *body) {
    macro* new_macro;
    new_macro = create_macro(name, body);
    if (*head == NULL) {
        *head = new_macro;
    } else {
        macro *i = *head;
        while (i->next != NULL) {
            i = i->next;
        }
        i->next = new_macro;
    }
}
macro* search_macro(macro *head, char *name, int *found) {
    while (head != NULL) {
        if (strcmp(name, head->name) == 0) {
            *found = 1;
            return head;
        }
        head = head->next;
    }
    return NULL;
}

/* free memory only after cheking with label names clash */
void release_macro(macro *head) {
    macro *i;
    macro *next;
    i = head;
    while (i != NULL) {
        next = i->next;
        free(i->name);
        free(i->body);
        free(i);
        i = next;
    }
}

/* for debugging only
void display_macros(macro *head) {
    macro *i = head;
    while (i != NULL) {
        printf("Macro Name: %s\n", i->name);
        printf("Macro Body: %s\n", i->body);
        printf("Macro :next %p\n", i->next);
        i = i->next;
    }
}*/

