#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "net.h"
#include "helper.h"

int main(int argc, char **argv) {
    srand(time(0));
    if (argc < 3) {
        printf("usage: ./neat train_x train_y test_x\n");
        return 0;
    }
    uint32_t lines = line_count(argv[1]);
    uint32_t inputs = word_count(argv[1]);
    uint32_t outputs = word_count(argv[2]);
    double **train_x = read_data(argv[1], lines, inputs);
    double **train_y = read_data(argv[2], lines, outputs);
    uint32_t lines_test = line_count(argv[3]);
    double **test_x = read_data(argv[3], lines_test, inputs);

    struct Net *net = new_net();
    populate(net, new_genome(inputs, outputs));
    struct Genome *best = NULL;
    for (int i = 0; i <= 500; i++) {
        best = test_net(net, lines, outputs, train_x, train_y);
        if (i % 50 == 0) {
            printf("\nepoch %d\n", i);
            print_genome(best);
        }
        new_epoch(net);
    }

    printf("\n\npredictions:\n");
    predict(best, lines_test, inputs, outputs, test_x);
    return 0;
}
