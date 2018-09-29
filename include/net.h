#ifndef NET_H
#define NET_H

#include "list.h"
#include "gene.h"
#include "genome.h"
#include "node.h"

struct Species {
    struct List *genomes;
};

struct Species* new_species(struct Genome *genome);
bool same_species(struct Species *species, struct Genome *genome);
double test_genome(struct Genome* genome, uint32_t rows, uint32_t inputs, uint32_t outputs, double **x, double **y);

#endif
