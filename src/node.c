#include <stdlib.h>
#include <stdio.h>
#include "node.h"
#include "gene.h"

struct Node* new_node(struct List *in_genes, enum node_type type) {
    struct Node *node = calloc(1, sizeof(struct Node));
    node->id = node_number();
    node->in_genes = in_genes;
    node->type = type;
}

uint32_t node_number() {
    static uint32_t num = 0;
    return ++num;
}

void print_node(struct Node *node) {
    printf("Node %d inputs:", node->id);
    if (node->in_genes) {
        struct ListItem *walk = node->in_genes->head;
        while (walk) {
            struct Gene *gene = walk->data;
            printf(" %d", gene->from);
            if (!gene->enabled) {
                printf("-");
            }
            walk = walk->next;
        }
    } else {
        printf(" NULL");
    }
    printf("\n");
}
