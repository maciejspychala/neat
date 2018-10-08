#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "net.h"
#include "helper.h"
#define DEBUG 0
#define INIT_COUNT 50
#define SPECIES_CLEAN_COUNT 10
#define GENOME_MAX_COUNT 200

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

int cmp_fitness(void *a, void* b) {
    struct Genome *g1 = a;
    struct Genome *g2 = b;

    if (g1->fitness > g2->fitness) {
        return 1;
    } else if (g1->fitness == g2->fitness) {
        return 0;
    } else {
        return -1;
    }
}

struct Net* new_net() {
    struct Net *net = calloc(1, sizeof(struct Net));
    net->species = new_list();
    return net;
}

void add_genome(struct Net *net, struct Genome *genome) {
    bool added = false;

    void try_to_add(void *species_data) {
        struct Species *species = species_data;
        if (same_species(species, genome) && !added) {
            add_data(species->genomes, genome);
            added = true;
        }
    }
    iterate_list(net->species, try_to_add);

    if (!added) {
        struct Species *species = new_species(genome);
        add_data(net->species, species);
    }
}

void populate(struct Net *net, struct Genome *genome) {
    for (int i = 0; i < INIT_COUNT - 1; i++) {
        struct Genome *new_genome = copy_genome(genome);
        evolve_genes_weights(new_genome);
        add_genome(net, new_genome);
    }
    add_genome(net, genome);
}

double test_species(struct Species *species, uint32_t rows, uint32_t inputs, uint32_t outputs, double **x, double **y) {
    double max = 0;
    void run_test_on_genome(void *genome_data) {
        struct Genome *genome = genome_data;
        double score = test_genome(genome, rows, inputs, outputs, x, y);
        if (score > max) {
            max = score;
        }
    }
    iterate_list(species->genomes, run_test_on_genome);
    return max;
}

struct Genome* test_net(struct Net *net, uint32_t rows, uint32_t inputs, uint32_t outputs, double **x, double **y) {
    struct Genome *max = 0;
    void run_test_on_species(void *species_data) {
        struct Species *species = species_data;
        test_species(species, rows, inputs, outputs, x, y);
        sort_list(species->genomes, cmp_fitness);
        struct Genome *genome = species->genomes->head->data;
        if(!max || genome->fitness > max->fitness) {
            max = genome;
        }
    }
    iterate_list(net->species, run_test_on_species);
    return max;
}

double test_genome(struct Genome *genome, uint32_t rows, uint32_t inputs, uint32_t outputs, double **x, double **y) {
    double score = 0;
    for (uint32_t i = 0; i < rows; i++) {
        calculate_output(genome, x[i]);
        double *out = collect_output(genome, outputs);
        if (DEBUG) {
            printf("for (");
            print_double_arr(x[i], inputs);
            printf(") received (");
            print_double_arr(out, outputs);
            printf(") should be (");
            print_double_arr(y[i], outputs);
            printf(")\n");
        }
        for (uint32_t j = 0; j < outputs; j++) {
            score += (out[j] - y[i][j]) * (out[j] - y[i][j]);
        }
    }
    score = 1.0 / (score + 1);
    genome->fitness = score;
    return score;
}

void clean_species(struct Species *species, uint32_t left) {
    struct ListItem *walk = get_item(species->genomes, left - 1);
    if (walk) {
        struct ListItem *tmp = walk;
        walk = walk->next;
        tmp->next = NULL;
    }
    while (walk) {
        destroy_genome(walk->data);
        struct ListItem *tmp = walk;
        walk = walk->next;
        free(tmp);
    }
}

struct Genome* random_genome(struct Species *species) {
    return get_item(species->genomes, rand() % list_size(species->genomes))->data;
}

struct Genome* new_child(struct Species *species) {
    double random = random_zero_to_one();
    struct Genome *child = NULL;
    if (random < 0.33) {
        child = child_add_connection(random_genome(species));
    } else if (random < 0.66) {
        child = crossover(random_genome(species), random_genome(species));
    } else {
        child = child_add_node(random_genome(species));
    }
    return child;
}

void new_epoch(struct Net *net) {
    static uint32_t epoch = 0;

    struct List *childs = new_list();
    void new_epoch_for_species(void *species_data) {
        struct Species *species = species_data;
        clean_species(species, SPECIES_CLEAN_COUNT);
        for (uint32_t i = 0; i < (GENOME_MAX_COUNT / list_size(net->species)); i++) {
            add_data(childs, new_child(species));
        }
        clean_species(species, 1);
    }
    iterate_list(net->species, new_epoch_for_species);

    void add_child(void *data) {
        struct Genome *child = data;
        if (rand() % 5) {
            evolve_genes_weights(child);
        }
        add_genome(net, child);
    }
    iterate_list(childs, add_child);

    printf("epoch: %d, species: %d ", epoch++, list_size(net->species));
}

double random_zero_to_one() {
    return rand() / (double) RAND_MAX;
}
