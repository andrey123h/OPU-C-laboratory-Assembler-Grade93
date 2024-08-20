#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

/* represent a node in data linked list */
typedef struct data_list {
    char* data_bnr; /* binary reprasentaion of the data */
    struct data_list *next;
} data_list;

data_list* create_data(char* data_bnr);
void add_data(data_list** head, char* data_bnr);
void print_data_list(data_list* head);
#endif