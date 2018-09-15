#ifndef NODE_H
#define NODE_H

#include <stdint.h>
#include "list.h"

struct Node {
    uint32_t id;
    struct List *in_genes;
};

struct Node* new_node(struct List *in_genes);
uint32_t node_number();
void print_node(struct Node *node);

#endif
