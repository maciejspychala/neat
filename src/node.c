#include <stdlib.h>
#include <stdio.h>
#include "node.h"
#include "gene.h"

struct Node* new_node(struct List *in_genes, enum node_type type) {
    struct Node *node = calloc(1, sizeof(struct Node));
    node->id = node_number();
    node->in_genes = in_genes;
    node->type = type;
    node->visited = false;
    node->value = 0.0;
    return node;
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
