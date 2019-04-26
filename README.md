Parallel Programming Project

IMPORTANT: Current implemented SUMMA algorithm only supports rank size that can be square rooted.
Therefore, all m, n, and k have to be divisable by sqrt(ranksize).

Input Matric: A m x k B k x n

There are three executable in this project:

1. SUMMA_NonBlocking.out -- Project_V1_0.c
2. SUMMA_Blocking.out -- Project_V1_1.c clcg4.c

3. Generate_TestCase.out -- generate.c

* SUMMA_NonBlocking.out:
Usage: <input file> <output file> <m> <n> <k>

Compilation: mpicc Project_V1_0.c -o SUMMA_NonBlocking.out -lm

However, we did not implement file output function. So, just input anything to avoid segmentation
fault.

This binary file is used in both vaildation and performance benchmark.

* SUMMA_Blocking.out:
Usage: <key> <m> <n> <k>

-<key>: Used as a seed in clcg4 library

Compilation: mpicc Project_V1_1.c clcg4.c -o SUMMA_Blocking.out -lm

This one is only used for performance benchmark.

* Generate_TestCase.out:
Compilation: gcc generate.c -o matrix
Usage: <outoutfile.txt> <n> <n> 
(only support square matrix now)


Vaildating that implemented SUMMA algorithm is correct:
We current use 8*8 small case to vaildate:

mpirun ./SUMMA_NonBlocking.out <generated 8 * 8 input file> 8 8 8


