#include <stdio.h>
#include "net.h"

int main() {
    int a = 14, b = 932;
    struct List *list = new_list();
    add_data(list, &a);
    add_data(list, &b);
    printf("size %d\n", list->size);
    struct ListItem *walk = list->head;
    while (walk) {
        printf("%d ", *(int*) walk->data);
        walk = walk->next;
    }
    return 0;
}
