#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "helper.h"

uint32_t line_count(char* filename) {
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

uint32_t word_count(char* filename) {
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

