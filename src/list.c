#include <stdlib.h>
#include <string.h>
#include "list.h"

struct List* new_list() {
    struct List *list = calloc(1, sizeof(struct List));
    return list;
}

struct ListItem* new_item(void *data) {
    struct ListItem *item = calloc(1, sizeof(struct ListItem));
    item->data = data;
    return item;
}

void add_data(struct List *list, void *data) {
    struct ListItem *item = new_item(data);
    struct ListItem *walk = list->head;
    if (!walk) {
        list->head = item;
    } else {
        while (walk->next) {
            walk = walk->next;
        }
        walk->next = item;
    }
    list->size++;
}

void push_data(struct List *list, void *data) {
    struct ListItem *item = new_item(data);
    item->next = list->head;
    list->head = item;
    list->size++;
}

void* pop_data(struct List *list) {
    struct ListItem* head = list->head;
    list->head = head->next;
    list->size--;
    return head->data;
}

struct ListItem* get_data(struct List *list, int index) {
    if (list->size < index) {
        return NULL;
    }
    struct ListItem *walk = list->head;
    while (index-- > 0) {
        walk = walk->next;
    }
    return walk->data;
}

void* fold(struct List *list, void* (*compare)(void *a, void *b)) {
    if (!list->head) {
        return NULL;
    }
    void *chosen = list->head->data;
    struct ListItem *walk = list->head;
    while (walk) {
        void *tmp = walk->data;
        chosen = compare(chosen, tmp);
        walk = walk->next;
    }
    return chosen;
}

struct List* copy_list(struct List *list, size_t struct_size) {
    struct List *new = new_list();
    struct ListItem *walk = list->head;
    while (walk) {
        void *data = calloc(1, struct_size);
        memcpy(data, walk->data, struct_size);
        add_data(new, data);
        walk = walk->next;
    }
    return new;
}
