#ifndef GENOME_H
#define GENOME_H

#include <stdint.h>
#include "list.h"

struct Genome {
    struct List *nodes;
};

struct Genome* new_genome(uint32_t input_nodes, uint32_t output_nodes);
float calculate_output(struct Genome* genome, float* input, uint32_t size);

#endif
