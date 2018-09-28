#ifndef HELPER_H
#define HELPER_H

#include <stdlib.h>

uint32_t line_count(char *filename);
uint32_t word_count(char *filename);
double** read_data(char *filename, uint32_t rows, uint32_t cols);

#endif
