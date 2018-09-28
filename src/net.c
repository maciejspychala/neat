#include <stdint.h>
#include <stdio.h>
#include "net.h"
#include "helper.h"

double test_genome(struct Genome* genome, uint32_t rows, uint32_t inputs, uint32_t outputs, double **x, double **y) {
    for (uint32_t i = 0; i < rows; i++) {
        calculate_output(genome, x[i]);
        double *out = collect_output(genome, outputs);
        printf("for (");
        print_double_arr(x[i], inputs);
        printf(") received (");
        print_double_arr(out, outputs);
        printf(") should be (");
        print_double_arr(y[i], outputs);
        printf(")\n");
    }
    return 0.1;
}
