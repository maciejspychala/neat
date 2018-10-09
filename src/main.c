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
    //uint32_t lines_test = line_count(argv[3]);
    //double **test_x = read_data(argv[3], lines_test, outputs);
    //double **test_y = read_data(argv[4], lines_test, outputs);

    struct Net *net = new_net();
    populate(net, new_genome(inputs, outputs));
    struct Genome *best = NULL;
    for (int i = 0; i < 500; i++) {
        best = test_net(net, lines, inputs, outputs, train_x, train_y);
        printf("%f\n", best->fitness);
        if (i % 20 == 0) {
            print_genome(best);
            test_genome(best, lines, inputs, outputs, train_x, train_y);
            char x;
            scanf("%c", &x);
            scanf("%c", &x);
        }
        new_epoch(net);
    }
    return 0;
}
