#include <stdio.h>
#include "net.h"

int main() {
    struct Genome *genome = new_genome(3, 1);
    evolve_gene(genome, 2, 4);
    add_data(find_node(genome, 5)->in_genes, new_gene(1, 5));
    add_data(find_node(genome, 5)->in_genes, new_gene(4, 5));
    struct ListItem *node = genome->nodes->head;
    float in[3] = { 1, 0.5, 0.2 };
    calculate_output(genome, in, 3);
    while (node) {
        print_node((struct Node*) node->data);
        node = node->next;
    }
    printf("\n\n");
    calculate_output(genome, in, 3);
    node = genome->nodes->head;
    while (node) {
        print_node((struct Node*) node->data);
        node = node->next;
    }
    return 0;
}
