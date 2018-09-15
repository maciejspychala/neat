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
    struct Gene *gene = new_gene(0, 2);
    struct Gene *another_gene = new_gene(1, 2);
    printf("gene id: %d\n", gene->id);
    printf("another_gene id: %d\n", another_gene->id);
    struct Genome* genome = new_genome(3);
    struct ListItem *node = genome->nodes->head;
    while (node) {
        print_node((struct Node*) node->data);
        node = node->next;
    }
    return 0;
}
