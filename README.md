# AG distribuido e paralelo

Algoritmo Paralelo e Distribuido de um AG para a solução do problema da mochila.

#### Execução do AG
> gcc ag.c -o ag -w
> ./ag

#### Execução do AG com OpenMP
> gcc ag.c -o ag -O -w
> export OMP_NUM_THREADS=<No Threads>
> ./ag

#### Execução do AG com MPI
> mpicc ag.c -o ag -O -w
> lamboot -v maquinas
> mpirun C ag
