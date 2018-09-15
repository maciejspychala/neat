#include <stdio.h>
#include "net.h"

int main() {
    struct Genome *genome = new_genome(5, 3);
    struct ListItem *node = genome->nodes->head;
    while (node) {
        print_node((struct Node*) node->data);
        node = node->next;
    }
    return 0;
}
