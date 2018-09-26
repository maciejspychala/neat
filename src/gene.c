#include <stdlib.h>
#include "gene.h"

struct Gene* new_gene(struct Genome *genome, uint32_t from, uint32_t to, float weight) {
    struct Gene *gene = calloc(1, sizeof(struct Gene));
    gene->id = gene_number();
    gene->from = from;
    gene->to = to;
    gene->weight = weight;
    gene->enabled = true;
    add_data(genome->global_genes, gene);
    return gene;
}

uint32_t gene_number() {
    static uint32_t num = 0;
    return ++num;
}

float random_weight() {
    return rand() / (float) RAND_MAX;
}

void evolve_weight(struct Gene *gene) {
    gene->weight = random_weight();
}
