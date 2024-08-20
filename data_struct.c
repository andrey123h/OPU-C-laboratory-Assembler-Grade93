#include "data_struct.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

data_list* create_data(char* data_bnr) {
    data_list* new_data = malloc(sizeof(data_list));
    new_data->data_bnr = malloc(strlen(data_bnr) + 1);
    strcpy(new_data->data_bnr, data_bnr);
    new_data->next = NULL;
    return new_data;
}

void add_data(data_list** head, char* data_bnr) {
    data_list* new_data;
     new_data = create_data(data_bnr);
    if (*head == NULL) {
        *head = new_data;
    } else {
        data_list* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_data;
    }
}

/* debbuging only */
void print_data_list(data_list* head) {
    data_list* current;
     current = head;
    while (current != NULL) {
        printf("Data: %s\n", current->data_bnr);
        current = current->next;
    }
}