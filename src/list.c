#include <stdlib.h>
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
