#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algoritmo.h"
#include "funcao.h"
#include "utils.h"

#define DEFAULT_RUNS 10

// Function to print the menu for algorithm selection
void print_menu()
{
	printf("\nEscolha o algoritmo:\n");
	printf("1. Trepa Colinas\n");
	printf("2. Evolutivo\n");
	printf("3. Híbrido\n");
}

// Prompts user for algorithm configuration options and updates parameters
void configure_algorithm(struct info *d)
{
	int tmp;

	print_menu();
	scanf("%d", &tmp);
	d->cfg.algorithm = (enum AlgorithmType)tmp;

	printf("\nEstrategia para solucoes invalidas:\n");
	printf("1. Penalizacao\n");
	printf("2. Reparacao\n");
	scanf("%d", &tmp);
	d->cfg.invalid_handling = (enum InvalidSolutionStrategy)tmp;

	if (d->cfg.algorithm == TREPA_COLINAS || d->cfg.algorithm == HIBRIDO)
	{
		printf("\nTipo de vizinhanca:\n");
		printf("1. Mudar uma moeda\n");
		printf("2. Mudar duas moedas\n");
		scanf("%d", &tmp);
		d->cfg.neighborhood = (enum NeighborhoodType)tmp;

		printf("\nAceitar solucoes de mesmo custo? (0/1):\n");
		scanf("%d", &d->cfg.accept_equal);
	}

	if (d->cfg.algorithm == HIBRIDO)
	{
		printf("\nAplicar trepa colinas na populacao inicial? (0/1):\n");
		scanf("%d", &d->cfg.hybrid_initial);
		printf("\nAplicar trepa colinas na populacao final? (0/1):\n");
		scanf("%d", &d->cfg.hybrid_final);
	}
}

void run_trepa_colinas(struct info EA_param, int mat[][2], int runs);
void run_evolutivo(struct info EA_param, int mat[][2], int runs);
void run_hibrido(struct info EA_param, int mat[][2], int runs);

int main(int argc, char *argv[])
{
	char nome_fich[100];
	struct info EA_param;
	int runs, mat[MAX_OBJ][2];

	if (argc == 3)
	{
		runs = atoi(argv[2]);
		strcpy(nome_fich, argv[1]);
	}
	else if (argc == 2)
	{
		runs = DEFAULT_RUNS;
		strcpy(nome_fich, argv[1]);
	}
	else
	{
		runs = DEFAULT_RUNS;
		printf("Nome do Ficheiro: ");
		fgets(nome_fich, sizeof(nome_fich), stdin);
		nome_fich[strcspn(nome_fich, "\n")] = 0;
	}

	if (runs <= 0)
		return 0;

	init_rand();
	EA_param = init_data(nome_fich);
	configure_algorithm(&EA_param);

	switch (EA_param.cfg.algorithm)
	{
	case TREPA_COLINAS:
		run_trepa_colinas(EA_param, mat, runs);
		break;
	case EVOLUTIVO:
		run_evolutivo(EA_param, mat, runs);
		break;
	case HIBRIDO:
		run_hibrido(EA_param, mat, runs);
		break;
	default:
		printf("Algoritmo desconhecido\n");
		return 1;
	}

	return 0;
}

// Executes hill climbing algorithm for specified number of runs
void run_trepa_colinas(struct info EA_param, int mat[][2], int runs)
{
	pchrom pop;
	chrom best_run, best_ever;
	int r, i, inv;
	float mbf = 0.0;

	best_ever.fitness = 999999;
	best_ever.valido = 0;

	for (r = 0; r < runs; r++)
	{
		pop = init_pop(EA_param);
		evaluate(pop, EA_param, mat);
		best_run = pop[0];
		trepa_colinas(pop, EA_param, mat);
		best_run = get_best(pop, EA_param, best_run);

		for (inv = 0, i = 0; i < EA_param.popsize; i++)
			if (pop[i].valido == 0)
				inv++;
		printf("\nRepeticao %d:", r + 1);
		write_best(best_run, EA_param);
		printf("\nPercentagem Invalidos: %f\n", 100 * (float)inv / EA_param.popsize);
		mbf += best_run.fitness;

		if (r == 0 || (best_run.valido && best_run.fitness < best_ever.fitness))
			best_ever = best_run;

		free(pop);
	}

	printf("\n\nMBF: %f\n", mbf / runs);
	printf("\nMelhor solucao encontrada");
	write_best(best_ever, EA_param);
}

// Executes evolutionary algorithm for specified number of runs
void run_evolutivo(struct info EA_param, int mat[][2], int runs)
{
	pchrom pop, parents;
	chrom best_run, best_ever;
	int r, i, inv, gen_actual;
	float mbf = 0.0;

	best_ever.fitness = 999999;
	best_ever.valido = 0;

	for (r = 0; r < runs; r++)
	{
		printf("Repeticao %d\n", r + 1);
		pop = init_pop(EA_param);
		parents = malloc(sizeof(chrom) * EA_param.popsize);
		if (parents == NULL)
		{
			printf("Erro na alocacao de memoria\n");
			exit(1);
		}
		evaluate(pop, EA_param, mat);
		best_run = pop[0];
		gen_actual = 1;

		while (gen_actual <= EA_param.numGenerations)
		{
			tournament_geral(pop, EA_param, parents);
			genetic_operators(parents, EA_param, pop);
			evaluate(pop, EA_param, mat);
			best_run = get_best(pop, EA_param, best_run);
			gen_actual++;
		}

		for (inv = 0, i = 0; i < EA_param.popsize; i++)
			if (pop[i].valido == 0)
				inv++;
		write_best(best_run, EA_param);
		printf("\nPercentagem Invalidos: %f\n", 100 * (float)inv / EA_param.popsize);
		mbf += best_run.fitness;

		if (r == 0 || (best_run.valido && best_run.fitness < best_ever.fitness))
			best_ever = best_run;

		free(parents);
		free(pop);
	}

	printf("\n\nMBF: %f\n", mbf / runs);
	printf("\nMelhor solucao encontrada");
	write_best(best_ever, EA_param);
}

// Executes hybrid algorithm combining hill climbing and evolutionary approaches
void run_hibrido(struct info EA_param, int mat[][2], int runs)
{
	pchrom pop, parents;
	chrom best_run, best_ever;
	int r, i, inv, gen_actual;
	float mbf = 0.0;

	best_ever.fitness = 999999;
	best_ever.valido = 0;

	for (r = 0; r < runs; r++)
	{
		printf("Repeticao %d\n", r + 1);
		pop = init_pop(EA_param);
		parents = malloc(sizeof(chrom) * EA_param.popsize);
		if (parents == NULL)
		{
			printf("Erro na alocacao de memoria\n");
			exit(1);
		}
		evaluate(pop, EA_param, mat);
		if (EA_param.cfg.hybrid_initial)
			trepa_colinas(pop, EA_param, mat);
		best_run = pop[0];
		gen_actual = 1;

		while (gen_actual <= EA_param.numGenerations)
		{
			tournament_geral(pop, EA_param, parents);
			genetic_operators(parents, EA_param, pop);
			evaluate(pop, EA_param, mat);
			best_run = get_best(pop, EA_param, best_run);
			gen_actual++;
		}

		if (EA_param.cfg.hybrid_final)
			trepa_colinas(pop, EA_param, mat);

		for (inv = 0, i = 0; i < EA_param.popsize; i++)
			if (pop[i].valido == 0)
				inv++;
		printf("\nRepeticao %d:", r);
		write_best(best_run, EA_param);
		printf("\nPercentagem Invalidos: %f\n", 100 * (float)inv / EA_param.popsize);
		mbf += best_run.fitness;

		if (r == 0 || (best_run.valido && best_run.fitness < best_ever.fitness))
			best_ever = best_run;

		free(parents);
		free(pop);
	}

	printf("\n\nMBF: %f\n", mbf / runs);
	printf("\nMelhor solucao encontrada");
	write_best(best_ever, EA_param);
}
