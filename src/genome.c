#include <stdlib.h>
#include "gene.h"
#include "genome.h"
#include "node.h"

struct Genome* new_genome(uint32_t input_nodes) {
    struct Genome *genome = calloc(1, sizeof(struct Genome));
    struct Node *out_node = new_node(new_list(), OUT);
    genome->nodes = new_list();
    for (uint32_t i = 0; i < input_nodes; i++) {
        struct Node *in_node = new_node(NULL, IN);
        add_data(genome->nodes, in_node);
        add_data(out_node->in_genes, new_gene(in_node->id, out_node->id));
    }
    add_data(genome->nodes, out_node);
    return genome;
}

float calculate_output(struct Genome* genome, float* input, uint32_t size) {
    return 1.0;
}
