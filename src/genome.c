#include <stdlib.h>
#include "gene.h"
#include "genome.h"

struct Genome* new_genome(uint32_t input_nodes) {
    struct Genome *genome = calloc(1, sizeof(struct Genome));
    genome->genes = new_list();
    for (uint32_t i = 0; i < input_nodes; i++) {
        struct Gene* gene = new_gene(i, input_nodes);
        add_data(genome->genes, gene);
    }
    return genome;
}

float calculate_output(struct Genome* genome, float* input, uint32_t size) {
    return 1.0;
}
