all:
	@gcc $(PROG).c -pthread -fopenmp -o main -Wall -lm -I.
	
run:
	@./main

debug:
	@gcc $(PROG).c -pthread -fopenmp -o main -Wall -lm -I.
	@valgrind --leak-check=full --show-leak-kinds=all -v ./main