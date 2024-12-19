#ifndef UTILS_H
#define UTILS_H

#include "algoritmo.h"

// Core utility functions for the optimization algorithms
int flip();                            // Generate random binary decision (0/1)
void init_rand();                      // Seed the random number generator
struct info init_data(char *filename); // Load problem configuration from file
pchrom init_pop(struct info d);        // Create initial population
int random_l_h(int min, int max);      // Generate random integer in [min, max]
float rand_01();                       // Generate random float in [0, 1]

#endif
