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
    void print_gene(void* gene_data) {
        struct Gene *gene = gene_data;
        if (gene->enabled) {
            printf(" %d", gene->from);
            printf("(%.5f)", gene->weight);
        }
    }
    iterate_list(node->in_genes, print_gene);
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

void calculate_node(struct Genome *genome, struct Node *node) {
    double value = 0.0;

    void add_gene_signal_to_value(void *gene_data) {
        struct Gene *gene = gene_data;
        if (gene->enabled) {
            value += gene->weight * find_node(genome, gene->from)->value;
        }
    }
    iterate_list(node->in_genes, add_gene_signal_to_value);

    node->value = sigmoid(value);
}
