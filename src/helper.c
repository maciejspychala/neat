#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "helper.h"

uint32_t line_count(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "cannot open file %s\n", filename);
    }
    uint32_t line_count = 0;
    for (char c = getc(file); c != EOF; c = getc(file)) {
        if (c == '\n') {
            line_count++;
        }
    }
    fclose(file);
    return line_count;
}

uint32_t word_count(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "cannot open file %s\n", filename);
    }
    uint32_t word_count = 0;
    bool word = false;
    for (char c = getc(file); c != EOF && c != '\n'; c = getc(file)) {
        if (c == ' ' || c == '\t') {
            if (word) {
                word = false;
                word_count++;
            }
        } else {
            if (!word) {
                word = true;
            }
        }
    }
    if (word == true) {
        word_count++;
    }
    fclose(file);
    return word_count;
}

double** read_data(char *filename, uint32_t rows, uint32_t cols) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "cannot open file %s\n", filename);
    }

    double **data = calloc(rows, sizeof(double*));
    for (uint32_t i = 0; i < rows; i++) {
        data[i] = calloc(cols, sizeof(double));
        for (uint32_t j = 0; j < cols; j++) {
            fscanf(file, "%lf", &data[i][j]);
        }
    }
    return data;
}

void print_double_arr(double *arr, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%lf", arr[i]);
    }
}
