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
    printf("test lines: %d\n", lines);
    double **data = read_data(argv[1], lines, inputs);
    struct Genome *genome = new_genome(inputs, outputs);
    print_genome(genome);
    return 0;
}
