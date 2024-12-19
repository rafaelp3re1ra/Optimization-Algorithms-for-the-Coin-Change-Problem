# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJ = main.o utils.o funcao.o algoritmo.o

# Build targets
all: coin_problem

# Link object files
coin_problem: $(OBJ)
	$(CC) $(CFLAGS) -o coin_problem $(OBJ)

# Compilation rules
main.o: main.c algoritmo.h funcao.h utils.h
	$(CC) $(CFLAGS) -c main.c

utils.o: utils.c algoritmo.h utils.h
	$(CC) $(CFLAGS) -c utils.c

funcao.o: funcao.c algoritmo.h funcao.h utils.h
	$(CC) $(CFLAGS) -c funcao.c

algoritmo.o: algoritmo.c algoritmo.h utils.h
	$(CC) $(CFLAGS) -c algoritmo.c

# Cleanup
clean:
	rm -f *.o coin_problem
