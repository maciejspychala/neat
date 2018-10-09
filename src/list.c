#include <stdlib.h>
#include <stdbool.h>
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

uint32_t list_size(struct List *list) {
    uint32_t size = 0;
    void inc_size(void *data) {
        (void)data;
        size++;
    }
    iterate_list(list, inc_size);
    return size;
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
}

void push_data(struct List *list, void *data) {
    struct ListItem *item = new_item(data);
    item->next = list->head;
    list->head = item;
}

void* pop_data(struct List *list) {
    struct ListItem* head = list->head;
    list->head = head->next;
    return head->data;
}

struct ListItem* get_item(struct List *list, uint32_t index) {
    if (list_size(list) < index) {
        return NULL;
    }
    struct ListItem *walk = list->head;
    while (index-- > 0) {
        walk = walk->next;
    }
    return walk;
}

void* fold(struct List *list, int (*compare)(void *a, void *b)) {
    if (!list->head) {
        return NULL;
    }
    void *chosen = list->head->data;
    struct ListItem *walk = list->head;
    while (walk) {
        void *tmp = walk->data;
        if (compare(chosen, tmp) < 1) {
            chosen = tmp;
        }
        walk = walk->next;
    }
    return chosen;
}

struct List* copy_list(struct List *list, size_t struct_size) {
    struct List *new = new_list();
    void copy_data(void *data) {
        void *new_data = calloc(1, struct_size);
        memcpy(new_data, data, struct_size);
        add_data(new, new_data);
    }
    iterate_list(list, copy_data);
    return new;
}

void sort_list(struct List *list, int (*compare)(void *a, void *b)) {
    if (!list->head) {
        return;
    }
    bool touched = false;
    do {
        touched = false;
        struct ListItem *walk = list->head;
        while (walk->next) {
            if (compare(walk->data, walk->next->data) < 0) {
                void *tmp = walk->data;
                walk->data = walk->next->data;
                walk->next->data = tmp;
                touched = true;
            }
            walk = walk->next;
        }
    } while (touched);
}

void destroy_list(struct List *list) {
    struct ListItem *walk = list->head;
    while (walk) {
        free(walk->data);
        struct ListItem *tmp = walk;
        walk = walk->next;
        free(tmp);
    }
    free(list);
    list = NULL;
}

void iterate_list(struct List *list, void (*func)(void *data)) {
    struct ListItem *walk = list->head;
    while (walk) {
        func(walk->data);
        walk = walk->next;
    }
}
