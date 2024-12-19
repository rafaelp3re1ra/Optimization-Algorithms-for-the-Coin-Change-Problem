#ifndef ALGORITMO_H
#define ALGORITMO_H

#define MAX_OBJ 1000 // Maximum number of objects

// Enumeration for algorithm types
enum AlgorithmType
{
    TREPA_COLINAS = 1,
    EVOLUTIVO = 2,
    HIBRIDO = 3
};

// Enumeration for invalid solution handling strategies
enum InvalidSolutionStrategy
{
    PENALTIES = 1,
    REPAIR = 2
};

// Enumeration for neighborhood types
enum NeighborhoodType
{
    CHANGE_ONE = 1,
    CHANGE_TWO = 2
};

// Configuration structure
struct config
{
    enum AlgorithmType algorithm;                  // Algorithm type
    enum InvalidSolutionStrategy invalid_handling; // Strategy for handling invalid solutions
    enum NeighborhoodType neighborhood;            // Neighborhood type
    int accept_equal;                              // For hill climbing: accept equal solutions (0 or 1)
    int hybrid_initial;                            // For hybrid: use initial hybridization (0 or 1)
    int hybrid_final;                              // For hybrid: use final hybridization (0 or 1)
};

// Structure to store parameters
struct info
{
    float popsize;        // Population size
    float pm;             // Mutation probability
    float pr;             // Recombination probability
    int tsize;            // Tournament size for parent selection
    float ro;             // Constant for evaluation with penalties
    int numGenes;         // Number of different coin types
    float target;         // Target value to achieve (V)
    float coins[MAX_OBJ]; // Array to store coin values
    int numGenerations;   // Number of generations
    struct config cfg;    // Configuration settings
};

// Individual (solution) structure
typedef struct individual chrom, *pchrom;

struct individual
{
    int p[MAX_OBJ]; // Number of each coin type used
    float fitness;  // Quality (number of coins used)
    int valido;     // Validity of the solution (1 if sum equals target, 0 otherwise)
    float sum;      // Total sum of coins
};

// Function declarations
void tournament(pchrom, struct info, pchrom);        // Tournament selection
void tournament_geral(pchrom, struct info, pchrom);  // General tournament selection
void genetic_operators(pchrom, struct info, pchrom); // Apply genetic operators
void crossover(pchrom, struct info, pchrom);         // Crossover operation
void mutation(pchrom, struct info);                  // Mutation operation
void mutacao_por_troca(pchrom, struct info);         // Swap mutation
int flip();                                          // Randomly return 0 or 1

#endif
