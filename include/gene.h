#ifndef GENE_H
#define GENE_H

#include <stdint.h>
#include <stdbool.h>
#include "genome.h"

struct Gene {
    uint32_t id;
    uint32_t from;
    uint32_t to;
    float weight;
    bool enabled;
};

struct Gene* new_gene(struct Genome *genome, uint32_t from, uint32_t to, float weight);
uint32_t gene_number();
float random_weight();
void evolve_weight(struct Gene *gene);

#endif
