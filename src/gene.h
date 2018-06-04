#ifndef GENE_H
#define GENE_H

#include <stdint.h>

struct Gene {
    uint32_t id;
    uint32_t from;
    uint32_t to;
};

struct Gene* new_gene();
uint32_t gene_number();

#endif
