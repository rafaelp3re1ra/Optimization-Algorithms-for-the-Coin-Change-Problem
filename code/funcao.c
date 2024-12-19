#define _CRT_SECURE_NO_WARNINGS 1
#include <math.h>
#include <stdlib.h>
#include "algoritmo.h"
#include "funcao.h"
#include "utils.h"

#define GENERATIONS_TC 10000
#define PROBGERAVIZ 0.8

// Penalization function for invalid solutions
float eval_penalized(int sol[], struct info d, float *sum)
{
    int i, total_coins = 0;
    float total_value = 0.0;

    for (i = 0; i < d.numGenes; i++)
    {
        total_coins += sol[i];
        total_value += sol[i] * d.coins[i];
    }

    *sum = total_value;

    if (fabs(total_value - d.target) != 0)
        return total_coins + fabs(total_value - d.target) * 1000; // Heavy penalty

    return total_coins;
}

// Repair function for invalid solutions
float eval_repaired(int sol[], struct info d, float *sum)
{
    int i, total_coins = 0;
    float total_value = 0.0;

    for (i = 0; i < d.numGenes; i++)
    {
        total_coins += sol[i];
        total_value += sol[i] * d.coins[i];
    }

    // Repair the solution to match the target
    if (total_value > d.target)
    {
        for (i = d.numGenes - 1; i >= 0 && total_value > d.target; i--)
        {
            while (sol[i] > 0 && total_value - d.coins[i] >= d.target)
            {
                sol[i]--;
                total_value -= d.coins[i];
                total_coins--;
            }
        }
    }
    else if (total_value < d.target)
    {
        for (i = 0; i < d.numGenes && total_value < d.target; i++)
        {
            while (total_value + d.coins[i] <= d.target)
            {
                sol[i]++;
                total_value += d.coins[i];
                total_coins++;
            }
        }
    }

    // Update sum after repair
    *sum = total_value;

    // Mark as valid only if the total equals the target within a small epsilon
    if (fabs(total_value - d.target) == 0)
        return total_coins;
    else
        return 999999; // Penalize as invalid
}

// Evaluate population
void evaluate(pchrom pop, struct info d, int mat[][2])
{
    int i;

    for (i = 0; i < d.popsize; i++)
    {
        if (d.cfg.invalid_handling == PENALTIES)
            pop[i].fitness = eval_penalized(pop[i].p, d, &pop[i].sum);
        else
            pop[i].fitness = eval_repaired(pop[i].p, d, &pop[i].sum);

        // Mark as valid only if the total equals the target within a small epsilon
        pop[i].valido = (fabs(pop[i].sum - d.target) == 0);
    }
}

// Generate neighbor solution
void gera_vizinho(int sol[], int solViz[], struct info d)
{
    int i, pos1, pos2;

    // Copy current solution
    for (i = 0; i < d.numGenes; i++)
        solViz[i] = sol[i];

    if (d.cfg.neighborhood == CHANGE_ONE)
    {
        // Neighborhood 1: Modify only one coin
        pos1 = random_l_h(0, d.numGenes - 1); // Choose a random coin

        if (rand_01() < 0.5 && solViz[pos1] > 0)
            solViz[pos1]--; // Decrease the quantity of this coin
        else
        {
            int max_coins = (int)(d.target / d.coins[pos1]) + 1; // Calculate maximum possible
            if (solViz[pos1] < max_coins)
                solViz[pos1]++; // Increase the quantity of this coin
        }
    }
    else // CHANGE_TWO
    {
        // Neighborhood 2: Modify two coins simultaneously
        pos1 = random_l_h(0, d.numGenes - 1); // Choose first coin
        do
        {
            pos2 = random_l_h(0, d.numGenes - 1); // Choose second coin
        } while (pos1 == pos2); // Ensure they are different coins

        // Swap between the two coins: decrease one and increase the other
        if (solViz[pos1] > 0)
        {
            solViz[pos1]--; // Decrease quantity of the first coin
            int max_coins = (int)(d.target / d.coins[pos2]) + 1;
            if (solViz[pos2] < max_coins)
                solViz[pos2]++; // Increase quantity of the second coin
        }
    }
}

// Hill climbing algorithm
void trepa_colinas(pchrom pop, struct info d, int mat[][2])
{
    int i, j;
    chrom atual, vizinho;
    float atual_fitness, vizinho_fitness;

    // For each solution in population
    for (i = 0; i < d.popsize; i++)
    {
        atual = pop[i];
        atual_fitness = atual.fitness;

        // Try to improve each solution
        for (j = 0; j < GENERATIONS_TC; j++)
        {
            gera_vizinho(atual.p, vizinho.p, d);

            // Evaluate neighbor
            if (d.cfg.invalid_handling == PENALTIES)
                vizinho_fitness = eval_penalized(vizinho.p, d, &vizinho.sum);
            else
                vizinho_fitness = eval_repaired(vizinho.p, d, &vizinho.sum);

            vizinho.fitness = vizinho_fitness;
            vizinho.valido = (fabs(vizinho.sum - d.target) == 0);

            // Accept if better (or equal if configured)
            if (vizinho.fitness < atual_fitness ||
                (d.cfg.accept_equal && vizinho.fitness == atual_fitness))
            {
                atual = vizinho;
                atual_fitness = vizinho_fitness;
            }
        }

        pop[i] = atual;
    }
}
