#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdint.h>

struct List {
    struct ListItem *head;
};

struct ListItem {
    struct ListItem *next;
    void *data;
};

struct List* new_list();
struct ListItem* new_item(void *data);
uint32_t list_size(struct List *list);
void add_data(struct List *list, void *data);
void push_data(struct List *list, void *data);
void* pop_data(struct List *list);
struct ListItem* get_item(struct List *list, uint32_t index);
struct List* copy_list(struct List *list, size_t struct_size);
void destroy_list(struct List *list);
void* fold(struct List *list, int (*compare)(void *a, void *b));
void sort_list(struct List *list, int (*compare)(void *a, void *b));
void iterate_list(struct List *list, void (*func)(void *data));

#endif
