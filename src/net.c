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
    for (int i = 0; i < INIT_COUNT - 1; i++) {
        struct Genome *new_genome = copy_genome(genome);
        evolve_genes_weights(new_genome);
        add_genome(net, new_genome);
    }
    add_genome(net, genome);
}

double test_species(struct Species *species, uint32_t rows, uint32_t inputs, uint32_t outputs, double **x, double **y) {
    struct ListItem *walk = species->genomes->head;
    double max = 0;
    while (walk) {
        struct Genome *genome = walk->data;
        double score = test_genome(genome, rows, inputs, outputs, x, y);
        if (score > max) {
            max = score;
        }
        walk = walk->next;
    }
    return max;
}

void test_net(struct Net *net, uint32_t rows, uint32_t inputs, uint32_t outputs, double **x, double **y) {
    struct ListItem *walk = net->species->head;
    double max = 0;
    while (walk) {
        struct Species *species = walk->data;
        double score = test_species(species, rows, inputs, outputs, x, y);
        if (score > max) {
            max = score;
        }
        sort_list(species->genomes, cmp_fitness);
        walk = walk->next;
    }
    printf("max score: %f\n", max);
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
        species->genomes->size = left;
    }
    while (walk) {
        destroy_genome(walk->data);
        struct ListItem *tmp = walk;
        walk = walk->next;
        free(tmp);
    }
}

struct Genome* random_genome(struct Species *species) {
    return get_item(species->genomes, rand() % species->genomes->size)->data;
}

struct Genome* new_child(struct Species *species) {
    double random = random_weight();
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
    struct ListItem *walk = net->species->head;
    struct List *childs = new_list();
    while (walk) {
        struct Species *species = walk->data;
        clean_species(species, SPECIES_CLEAN_COUNT);
        for (int i = 0; i < (GENOME_MAX_COUNT / net->species->size); i++) {
            add_data(childs, new_child(species));
        }
        clean_species(species, 1);
        walk = walk->next;
    }
    struct ListItem *child_walk = childs->head;
    while (child_walk) {
        struct Genome *child = child_walk->data;
        if (rand() % 5) {
            evolve_genes_weights(child);
        }
        add_genome(net, child);
        child_walk = child_walk->next;
    }
    printf("epoch number: %d, species count: %d\n", epoch++, net->species->size);
    print_genome(((struct Species*)net->species->head->data)->genomes->head->data);
}
