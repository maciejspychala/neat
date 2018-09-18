#ifndef GENOME_H
#define GENOME_H

#include <stdint.h>
#include "list.h"

struct Genome {
    struct List *nodes;
};

struct Genome* new_genome(uint32_t input_nodes, uint32_t output_nodes);
float calculate_output(struct Genome* genome, float* input, uint32_t size);
struct Node* find_node(struct Genome* genome, uint32_t node);
void evolve_gene(struct Genome* genome, uint32_t in_id, uint32_t out_id);

#endif
