#include <stdlib.h>
#include <stdio.h>
#include "node.h"
#include "gene.h"

struct Node* create_node(struct List *in_genes, enum node_type type, uint32_t id) {
    struct Node *node = calloc(1, sizeof(struct Node));
    node->id = id;
    node->in_genes = in_genes;
    node->type = type;
    node->visited = false;
    node->value = 0.0;
    return node;
}

struct Node* new_node(struct List *in_genes, enum node_type type) {
    return create_node(in_genes, type, node_number());
}

struct Node* new_node_with_id(uint32_t id) {
    return create_node(new_list(), HIDDEN, id);
}

uint32_t node_number() {
    static uint32_t num = 0;
    return ++num;
}

void print_node(struct Node *node) {
    printf("Node %d, value: %f, inputs:", node->id, node->value);
    if (node->in_genes) {
        struct ListItem *walk = node->in_genes->head;
        while (walk) {
            struct Gene *gene = walk->data;
            printf(" %d", gene->from);
            if (!gene->enabled) {
                printf("-");
            }
            printf("(%.5f)", gene->weight);
            walk = walk->next;
        }
    } else {
        printf(" NULL");
    }
    printf("\n");
}

struct Gene* find_gene(struct Node *node, uint32_t from_id) {
    struct ListItem *walk = node->in_genes->head;
    while(walk) {
        if (((struct Gene*) walk->data)->from == from_id) {
            return walk->data;
        }
        walk = walk->next;
    }
    return NULL;
}
