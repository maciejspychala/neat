#include "list.h"
#include "stdlib.h"

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
    add_item(list, item);
}

void add_item(struct List *list, struct ListItem *item) {
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