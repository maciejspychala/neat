#include <stdlib.h>
#include "node.h"

struct Node* new_node(struct List *in_genes) {
    struct Node *node = calloc(1, sizeof(struct Node));
    node->id = node_number();
    node->in_genes = in_genes
}

uint32_t node_number() {
    static uint32_t num = 0;
    return ++num;
}
