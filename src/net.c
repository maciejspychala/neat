#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "net.h"
#include "helper.h"
#define DEBUG 1
#define GENOME_COUNT 50

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

struct Net* new_net() {
    struct Net *net = calloc(1, sizeof(struct Net));
    net->species = new_list();
    return net;
}

void add_genome(struct Net *net, struct Genome *genome) {
    struct ListItem *walk = net->species->head;
    bool added = false;
    while (walk) {
        struct Species *species = walk->data;
        if (same_species(species, genome)) {
            add_data(species->genomes, genome);
            added = true;
            break;
        }
        walk = walk->next;
    }
    if (!added) {
        struct Species *species = new_species(genome);
        add_data(net->species, species);
    }
}

void populate(struct Net *net, struct Genome *genome) {
    for (int i = 0; i < GENOME_COUNT - 1; i++) {
        struct Genome *new_genome = copy_genome(genome);
        evolve_genes_weights(new_genome);
        add_genome(net, new_genome);
    }
    add_genome(net, genome);
}

void test_species(struct Species *species, uint32_t rows, uint32_t inputs, uint32_t outputs, double **x, double **y) {
    struct ListItem *walk = species->genomes->head;
    while (walk) {
        struct Genome *genome = walk->data;
        test_genome(genome, rows, inputs, outputs, x, y);
        walk = walk->next;
    }
}

void test_net(struct Net *net, uint32_t rows, uint32_t inputs, uint32_t outputs, double **x, double **y) {
    struct ListItem *walk = net->species->head;
    while (walk) {
        struct Species *species = walk->data;
        test_species(species, rows, inputs, outputs, x, y);
        walk = walk->next;
    }
}

double test_genome(struct Genome *genome, uint32_t rows, uint32_t inputs, uint32_t outputs, double **x, double **y) {
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
    score = 1.0 / (score + 0.0000000001);
    genome->fitness = score;
    return score;
}
