#ifndef NODE_H
#define NODE_H

#include <stdint.h>
#include "list.h"

enum node_type {
    IN,
    HIDDEN,
    OUT
};
struct Node {
    uint32_t id;
    enum node_type type;
    struct List *in_genes;
};

struct Node* new_node(struct List *in_genes, enum node_type type);
uint32_t node_number();
void print_node(struct Node *node);
struct Gene* find_gene(struct Node *node, uint32_t in_id);

#endif
