#ifndef GENOME_H
#define GENOME_H

#include <stdint.h>
#include "list.h"

struct Genome {
    double fitness;
    struct List *nodes;
    struct List *global_genes;
    uint32_t input_nodes;
    uint32_t output_nodes;
};

struct Genome* new_genome(uint32_t input_nodes, uint32_t output_nodes);
void calculate_output(struct Genome *genome, double *input);
struct Node* find_node(struct Genome *genome, uint32_t id);
struct Genome* child_add_connection(struct Genome *genome);
struct Genome* child_add_node(struct Genome *genome);
void evolve_gene(struct Genome *genome, uint32_t in_id, uint32_t out_id);
void evolve_genes_weights(struct Genome *genome);
void print_genome(struct Genome *genome);
void add_gene(struct Genome *genome, uint32_t in, uint32_t out, double weight);
struct Genome* copy_genome(struct Genome *genome);
struct Genome* crossover(struct Genome *better, struct Genome *worse);
double* collect_output(struct Genome *genome, uint32_t cols);
double distance(struct Genome *g1, struct Genome *g2);
void destroy_genome(struct Genome *genome);

#endif
