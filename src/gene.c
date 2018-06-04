#include "gene.h"
#include "stdlib.h"

struct Gene* new_gene() {
    struct Gene *gene = calloc(1, sizeof(struct Gene));
    gene->id = gene_number();
    return gene;
}

uint32_t gene_number() {
    static uint32_t num = 0;
    return ++num;
}

