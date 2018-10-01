#ifndef NET_H
#define NET_H

#include "list.h"
#include "gene.h"
#include "genome.h"
#include "node.h"

struct Species {
    struct List *genomes;
};

struct Net {
    struct List *species;
};

struct Species* new_species(struct Genome *genome);
bool same_species(struct Species *species, struct Genome *genome);
struct Net* new_net();
void add_genome(struct Net *net, struct Genome *genome);
void populate(struct Net *net, struct Genome *genome);
double test_genome(struct Genome* genome, uint32_t rows, uint32_t inputs, uint32_t outputs, double **x, double **y);

#endif
