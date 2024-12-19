# Coin Change Problem - Optimization Algorithms



This repository presents an academic project developed to solve the **Coin Change Problem** using different optimization techniques. The main goal is to minimize the number of coins used to reach a target value.



## Overview



The project explores three main approaches:



1. **Local Search Algorithm (Hill-Climbing):**

   - Uses penalties or repairs to handle invalid solutions.

   - Implements two neighborhoods: **Change One** and **Change Two**.



2. **Evolutionary Algorithm:**

   - Includes genetic operators such as recombination and mutation.

   - Represents each solution as a chromosome indicating the quantity of coins per denomination.



3. **Hybrid Approach:**

   - Combines Hill-Climbing with the Evolutionary Algorithm to explore the solution space and refine them locally.



## Repository Structure



- **Headers and Implementations:**

  - `utils.h` and `utils.c`: Auxiliary functions.

  - `funcao.h` and `funcao.c`: Core functions for solution evaluation.

  - `algoritmo.h` and `algoritmo.c`: Implementation of optimization algorithms.

  - `main.c`: Main file to run the experiments.



- **Test Data:**

  - Files like `file1.txt`, `file2.txt`, etc., contain the test scenarios used in the experiments.



- **Makefile:**

  - Contains rules to compile the project.



- **Report:**

  - [`Report.pdf`](Relatorio.pdf) details the development, experiments, and obtained results.

- **Experimental Results:**

  - [`Results.xlsx`](Resultados.xlsx) details the results in a excel file.

## How to Compile and Run



1. Ensure you have a C compiler installed (e.g., GCC).

2. In the main directory, run the following command to compile the project:

   ```bash

   make



3. After compilation, run the program:

   ```

   ./coin_problem

   ```

4. The results will be displayed in the console.



## Key Results



1. **Hill-Climbing:**

   * Best performance with 10,000 iterations using Neighborhood 2 with Repair and accepting same cost solutions.

2. **Evolutionary Algorithm:**

   * Uniform Recombination with Swap Mutation delivered the best results.

3. **Hybrid Approach:**

   * Consistently outperformed individual methods.



## References



For more details, check the technical report and explore the source code.



## Authors



- [Rafael Pereira](https://github.com/rafaelp3re1ra)

- [Fábio Oliveira](https://github.com/fabiooliv3ira)
