#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "net.h"
#include "helper.h"
#define DEBUG 1

struct Species* new_species(struct Genome *genome) {
    struct Species *species = calloc(1, sizeof(struct Species));
    species->genomes = new_list();
    add_data(species->genomes, genome);
    return species;
}

bool same_species(struct Species *species, struct Genome *genome) {
    struct Genome *leader = species->genomes->head->data;
    double dist = distance(leader, genome);
    return dist < 1.2;
}

double test_genome(struct Genome* genome, uint32_t rows, uint32_t inputs, uint32_t outputs, double **x, double **y) {
    double score = 0;
    for (uint32_t i = 0; i < rows; i++) {
        calculate_output(genome, x[i]);
        double *out = collect_output(genome, outputs);
#ifdef DEBUG
        printf("for (");
        print_double_arr(x[i], inputs);
        printf(") received (");
        print_double_arr(out, outputs);
        printf(") should be (");
        print_double_arr(y[i], outputs);
        printf(")\n");
#endif
        for (uint32_t j = 0; j < outputs; j++) {
            score += (out[j] - y[i][j]) * (out[j] - y[i][j]);
        }
    }
    score = 1 - score;
    genome->fitness = score;
    return score;
}
