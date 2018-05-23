#include "list.h"
#include "stdlib.h"

struct ListItem* new_item(void *data) {
    struct ListItem *item = malloc(sizeof(struct ListItem));
    item->data = data;
    return item;
}
