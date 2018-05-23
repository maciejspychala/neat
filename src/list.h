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

struct ListItem* new_item(void *data);

#endif
