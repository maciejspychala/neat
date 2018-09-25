#ifndef LIST_H
#define LIST_H

struct List {
    struct ListItem *head;
    int size;
};

struct ListItem {
    struct ListItem *next;
    void *data;
};

struct List* new_list();
struct ListItem* new_item(void *data);
void add_data(struct List *list, void *data);
void add_item(struct List *list, struct ListItem *item);
void push_data(struct List *list, void *data);
void push_item(struct List *list, struct ListItem *item);
void* pop_data(struct List *list);
struct ListItem* pop_item(struct List *list);
struct ListItem* get_item(struct List *list, int index);

#endif
