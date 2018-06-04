#include <stdlib.h>
#include "gene.h"

struct Gene* new_gene(uint32_t from, uint32_t to) {
    struct Gene *gene = calloc(1, sizeof(struct Gene));
    gene->id = gene_number();
    gene->from = from;
    gene->to = to;
    gene->weight = 1.0;
    return gene;
}

uint32_t gene_number() {
    static uint32_t num = 0;
    return ++num;
}

