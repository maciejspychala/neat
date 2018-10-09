#ifndef NET_H
#define NET_H

#include "list.h"
#include "gene.h"
#include "genome.h"
#include "node.h"

#define GENOME_MAX_COUNT 150
#define DISTANCE_WEIGHTS 0.4
#define DISTANCE_TRESHOLD 0.9
#define MUTATE_WEIGHT 0.8
#define WEIGHT_PERTRUB 0.90
#define WEIGHT_PERTRUB_MAX 0.1
#define NEW_NODE 0.03
#define NEW_CONNECTION 0.1

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
struct Genome* test_net(struct Net *net, uint32_t rows, uint32_t outputs, double **x, double **y);
double test_genome(struct Genome* genome, uint32_t rows, uint32_t outputs, double **x, double **y);
void clean_species(struct Species *species, uint32_t left);
void new_epoch(struct Net *net);
double random_zero_to_one();
void predict(struct Genome *genome, uint32_t rows, uint32_t inputs, uint32_t outputs, double **x);

#endif
