#include <stdio.h>
#include "net.h"

int main() {
    struct Genome *genome = new_genome(5, 3);
    struct ListItem *node = genome->nodes->head;
    evolve_gene(genome, 4, 8);
    float input[5] = { 1.0, 0.4, 0.2, 0.5, 0.8 };
    calculate_output(genome, input, 5);
    while (node) {
        print_node((struct Node*) node->data);
        node = node->next;
    }
    return 0;
}
