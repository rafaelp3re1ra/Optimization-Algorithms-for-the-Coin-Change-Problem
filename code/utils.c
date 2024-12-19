#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "algoritmo.h"
#include "utils.h"

// Initialize random number generator
void init_rand()
{
	srand((unsigned)time(NULL));
}

// Read problem parameters and data from file
struct info init_data(char *filename)
{
	struct info d;
	FILE *f;
	int i;

	f = fopen(filename, "rt");
	if (!f)
	{
		printf("File not found\n");
		exit(1);
	}

	// Read problem parameters
	fscanf(f, "%d %f", &d.numGenes, &d.target);

	// Read coin values
	for (i = 0; i < d.numGenes; i++)
		fscanf(f, "%f", &d.coins[i]);

	// Set default algorithm parameters
	d.popsize = 100;
	d.pm = 0.001;
	d.pr = 0.7;
	d.tsize = 2;
	d.numGenerations = 2500;

	// Initialize config defaults
	d.cfg.algorithm = TREPA_COLINAS;
	d.cfg.invalid_handling = REPAIR;
	d.cfg.neighborhood = CHANGE_ONE;
	d.cfg.accept_equal = 0;
	d.cfg.hybrid_initial = 0;
	d.cfg.hybrid_final = 0;

	fclose(f);
	return d;
}

// Simulate a coin flip, returning 0 or 1
int flip()
{
	return (((float)rand()) / RAND_MAX) < 0.5 ? 0 : 1;
}

// Get the best solution found so far
chrom get_best(pchrom pop, struct info d, chrom best)
{
	int i;

	for (i = 0; i < d.popsize; i++)
	{
		if (best.fitness > pop[i].fitness)
			best = pop[i];
	}
	return best;
}

// Return a random integer between min and max
int random_l_h(int min, int max)
{
	return min + rand() % (max - min + 1);
}

// Return a random float between 0 and 1
float rand_01()
{
	return ((float)rand()) / RAND_MAX;
}

// Print the best solution to the console
void write_best(chrom x, struct info d)
{
	int i;
	printf("\nSolução encontrada:\n");

	// Print number of each coin type
	for (i = 0; i < d.numGenes; i++)
		if (x.p[i] > 0)
			printf("%d moedas de %.2f\n", x.p[i], d.coins[i]);

	printf("Valor total: %.2f (objetivo: %.2f)\n", x.sum, d.target);
	printf("Número total de moedas: %d\n", (int)x.fitness);

	// Only print cost if it's a valid solution
	if (x.valido)
		printf("Custo final: %d\n", (int)x.fitness);
}
