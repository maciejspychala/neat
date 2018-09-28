#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "net.h"

int main() {
    srand(time(0));
    struct Genome *genome = new_genome(3, 1);
    evolve_gene(genome, 2, 5);
    double in[] = { 1, 0.5, 0.2 };
    calculate_output(genome, in);
    print_genome(genome);
    printf("\n\n");

    struct Genome *new_genome = copy_genome(genome);
    evolve_genes_weights(genome);
    calculate_output(genome, in);
    print_genome(genome);
    printf("\n\n");

    calculate_output(new_genome, in);
    print_genome(new_genome);
    printf("\n\n");


    struct Genome *child = crossover(genome, new_genome);
    calculate_output(child, in);
    print_genome(child);
    return 0;
}
