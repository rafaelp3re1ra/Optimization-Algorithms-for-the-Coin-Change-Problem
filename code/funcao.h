#ifndef FUNCAO_H
#define FUNCAO_H

#include "algoritmo.h"

// Function declarations
void evaluate(pchrom, struct info, int mat[][2]);      // Evaluate the fitness of each individual
void trepa_colinas(pchrom, struct info, int mat[][2]); // Hill climbing algorithm
chrom get_best(pchrom, struct info, chrom best);       // Get the best individual from the population
void write_best(chrom, struct info d);                 // Write the best individual to a file

// Declare new functions if any
float eval_repaired(int sol[], struct info d, float *sum); // Evaluate a repaired solution

#endif
