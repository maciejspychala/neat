#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "net.h"
#include "helper.h"

int main(int argc, char **argv) {
    srand(time(0));
    if (argc < 3) {
        printf("usage: ./neat in_values out_values\n");
        return 0;
    }
    uint32_t lines = line_count(argv[1]);
    uint32_t inputs = word_count(argv[1]);
    uint32_t outputs = word_count(argv[2]);
    double **train_x = read_data(argv[1], lines, inputs);
    double **train_y = read_data(argv[2], lines, outputs);
    struct Genome *genome = new_genome(inputs, outputs);
    double score = test_genome(genome, lines, inputs, outputs, train_x, train_y);

    struct Genome *copy = copy_genome(genome);
    printf("diff %lf\n", distance(genome, copy));
    evolve_genes_weights(copy);
    printf("diff %lf\n", distance(genome, copy));

    printf("score: %lf\n", score);

    struct Species *species = new_species(genome);
    printf("same species? %d\n", same_species(species, copy));

    return 0;
}
