all:
	@mpicc matriz_estat_mpi.c -o mpi -lm
	@gcc matriz_estat_seq.c -o seq -lm

run:
	@mpirun mpi -np 1 mpi < entrada.txt

run-seq:
	@./seq < entrada.txt