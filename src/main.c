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
    printf("\n2nd element: %d\n", *(int*) get_item(list, 1)->data);
    printf("if 10th elements is NULL? %d\n", get_item(list, 10) == NULL);
    return 0;
}
