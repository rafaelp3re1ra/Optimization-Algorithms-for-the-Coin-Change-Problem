#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include "algoritmo.h"
#include "utils.h"
#include <math.h>

// Creates initial population with feasible random solutions
pchrom init_pop(struct info d)
{
	int i, j;
	pchrom pop = malloc(d.popsize * sizeof(chrom));

	if (!pop)
	{
		printf("Erro ao alocar memória para a população\n");
		exit(1);
	}

	for (i = 0; i < d.popsize; i++)
	{
		float total = 0.0;
		for (j = 0; j < d.numGenes; j++)
		{
			int max_coins = (int)(d.target / d.coins[j]);
			pop[i].p[j] = random_l_h(0, max_coins);
			total += pop[i].p[j] * d.coins[j];
		}

		// Adjust the solution to meet the target value
		for (j = 0; j < d.numGenes && fabs(total - d.target) != 0; j++)
		{
			if (total < d.target)
			{
				int max_add = (int)((d.target - total) / d.coins[j]);
				int add = random_l_h(0, max_add);
				pop[i].p[j] += add;
				total += add * d.coins[j];
			}
			else if (total > d.target)
			{
				int max_sub = (int)((total - d.target) / d.coins[j]);
				int sub = random_l_h(0, max_sub);
				if (pop[i].p[j] >= sub)
				{
					pop[i].p[j] -= sub;
					total -= sub * d.coins[j];
				}
			}
		}

		// Ensure the solution is valid
		pop[i].valido = fabs(total - d.target) == 0;
		pop[i].sum = total;
		pop[i].fitness = 0;
		for (j = 0; j < d.numGenes; j++)
		{
			pop[i].fitness += pop[i].p[j];
		}
	}

	return pop;
}

// Selects parents using tournament selection of size d.tsize
void tournament_geral(pchrom pop, struct info d, pchrom parents)
{
	int i, j, k, sair, best, *pos;

	pos = malloc(d.tsize * sizeof(int));
	// Perform popsize tournaments
	for (i = 0; i < d.popsize; i++)
	{
		// Select tsize different solutions for the tournament
		for (j = 0; j < d.tsize; j++)
		{
			do
			{
				pos[j] = random_l_h(0, d.popsize - 1);
				// Check if the new position is equal to any of the previously chosen positions
				sair = 0;
				for (k = 0; k < j; k++)
				{
					if (pos[k] == pos[j])
						sair = 1;
				}
			} while (sair);
			// Store the position of the best solution in the tournament
			if (j == 0 || pop[pos[j]].fitness < pop[pos[best]].fitness) // Minimization problem
				best = j;
		}
		parents[i] = pop[pos[best]];
	}
	free(pos);
}

// Applies crossover and mutation to create new offspring
void genetic_operators(pchrom parents, struct info d, pchrom offspring)
{
	// Choose the type of recombination to use:
	crossover(parents, d, offspring); // Single-point crossover
	// recombinacao_uniforme(parents, d, offspring); // Uniform crossover

	// Binary mutation
	// mutation(offspring, d);
	mutacao_por_troca(offspring, d);
}

// Performs single-point crossover between pairs of parents
void crossover(pchrom parents, struct info d, pchrom offspring)
{
	int i, j, point;

	for (i = 0; i < d.popsize; i += 2)
	{
		if (rand_01() < d.pr)
		{
			point = random_l_h(0, d.numGenes - 1);
			for (j = 0; j < point; j++)
			{
				offspring[i].p[j] = parents[i].p[j];
				offspring[i + 1].p[j] = parents[i + 1].p[j];
			}
			for (j = point; j < d.numGenes; j++)
			{
				offspring[i].p[j] = parents[i + 1].p[j];
				offspring[i + 1].p[j] = parents[i].p[j];
			}
		}
		else
		{
			offspring[i] = parents[i];
			offspring[i + 1] = parents[i + 1];
		}
	}
}

// Performs uniform crossover between pairs of parents
void recombinacao_uniforme(pchrom parents, struct info d, pchrom offspring)
{
	int i, j;

	for (i = 0; i < d.popsize; i += 2)
	{
		if (rand_01() < d.pr)
		{
			for (j = 0; j < d.numGenes; j++)
			{
				if (flip() == 1)
				{
					offspring[i].p[j] = parents[i].p[j];
					offspring[i + 1].p[j] = parents[i + 1].p[j];
				}
				else
				{
					offspring[i].p[j] = parents[i + 1].p[j];
					offspring[i + 1].p[j] = parents[i].p[j];
				}
			}
		}
		else
		{
			offspring[i] = parents[i];
			offspring[i + 1] = parents[i + 1];
		}
	}
}

// Performs mutation by randomly adjusting coin quantities
void mutation(pchrom offspring, struct info d)
{
	int i, j;

	for (i = 0; i < d.popsize; i++)
	{
		if (rand_01() < d.pm)
		{
			j = random_l_h(0, d.numGenes - 1);
			if (rand_01() < 0.5 && offspring[i].p[j] > 0)
			{
				offspring[i].p[j]--;
			}
			else
			{
				int max_coins = (int)(d.target / d.coins[j]);
				if (offspring[i].p[j] < max_coins)
					offspring[i].p[j]++;
			}
		}
	}
}

// Performs mutation by swapping coin quantities between positions
void mutacao_por_troca(pchrom offspring, struct info d)
{
	int i, pos1, pos2, aux;

	for (i = 0; i < d.popsize; i++)
		if (rand_01() < d.pm)
		{
			pos1 = random_l_h(0, d.numGenes - 1);
			do
				pos2 = random_l_h(0, d.numGenes - 1);
			while (pos2 == pos1); // Ensure different positions

			aux = offspring[i].p[pos1];
			offspring[i].p[pos1] = offspring[i].p[pos2];
			offspring[i].p[pos2] = aux;
		}
}