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

    struct Net *net = new_net();
    populate(net, new_genome(inputs, outputs));
    printf("%d\n", ((struct Species*) net->species->head->data)->genomes->size);

    test_net(net, lines, inputs, outputs, train_x, train_y);
    struct Species *s = net->species->head->data;
    struct ListItem *walk = s->genomes->head;
    while (walk) {
        struct Genome *g = walk->data;
        printf("%f\n", g->fitness);
        walk = walk->next;
    }
    printf("\n\n\n");
    clean_species(s, 5);
    test_net(net, lines, inputs, outputs, train_x, train_y);
    walk = s->genomes->head;
    while (walk) {
        struct Genome *g = walk->data;
        printf("%f\n", g->fitness);
        walk = walk->next;
    }
    return 0;
}
