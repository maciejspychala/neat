#include <stdlib.h>
#include "gene.h"
#include "genome.h"
#include "node.h"

struct Genome* new_genome(uint32_t input_nodes, uint32_t output_nodes) {
    struct Genome *genome = calloc(1, sizeof(struct Genome));
    genome->nodes = new_list();

    for (uint32_t i = 0; i < input_nodes; i++) {
        add_data(genome->nodes, new_node(NULL, IN));
    }

    for (uint32_t i = 0; i < output_nodes; i++) {
        struct Node *out = new_node(new_list(), OUT);

        struct ListItem *walk = genome->nodes->head;
        while (walk) {
            if (((struct Node*) walk->data)->type == IN) {
                add_data(out->in_genes, new_gene(((struct Node*) walk->data)->id, out->id));
            }
            walk = walk->next;
        }
        add_data(genome->nodes, out);
    }
    return genome;
}

float calculate_output(struct Genome* genome, float* input, uint32_t size) {
    struct ListItem *walk = genome->nodes->head;
    while (walk) {
        struct Node *node = walk->data;
        node->visited = false;
        node->value = 0.0;
        if (node->type == IN) {
            node->value = input[node->id - 1];
            node->visited = true;
        }
        walk = walk->next;
    }
}

struct Node* find_node(struct Genome* genome, uint32_t node) {
    struct ListItem *walk = genome->nodes->head;
    while(walk) {
        if (((struct Node*) walk->data)->id == node) {
            return walk->data;
        }
        walk = walk->next;
    }
    return NULL;
}

void evolve_gene(struct Genome* genome, uint32_t in_id, uint32_t out_id) {
    struct Node *out = find_node(genome, out_id);
    struct Gene *gene = find_gene(out, in_id);
    gene->enabled = false;

    struct Node *inter = new_node(new_list(), HIDDEN);
    add_data(inter->in_genes, new_gene(in_id, inter->id));
    add_data(out->in_genes, new_gene(inter->id, out->id));
    add_data(genome->nodes, inter);
}
