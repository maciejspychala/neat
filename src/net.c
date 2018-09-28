#include <stdint.h>
#include <stdio.h>
#include "net.h"

double test_genome(struct Genome* genome, uint32_t rows, double **x, uint32_t cols, double **y) {
    for (uint32_t i = 0; i < rows; i++) {
        calculate_output(genome, x[i]);
        double *out = collect_output(genome, cols);
        print_genome(genome);
        for (uint32_t j = 0; j < cols; j++) {
            printf("%lf ", out[j]);
        }
        printf("\n");
        y[0][0] += 1;
    }
    return 0.1;
}
