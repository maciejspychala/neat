#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "net.h"

int main() {
    srand(time(0));
    struct Genome *genome = new_genome(3, 1);
    evolve_gene(genome, 2, 4);
    add_data(find_node(genome, 5)->in_genes, new_gene(1, 5, random_weight()));
    add_data(find_node(genome, 5)->in_genes, new_gene(4, 5, random_weight()));
    float in[] = { 1, 0.5, 0.2 };
    calculate_output(genome, in);
    print_genome(genome);
    printf("\n\n");

    calculate_output(genome, in);
    print_genome(genome);
    return 0;
}
