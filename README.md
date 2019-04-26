<h1>Parallel Programming Project</h1>

test1.txt 8*8 
test2.txt 16*16
test3.txt 64*64
test4.txt 1024*1024

result should equal to second matrix, i.e, B=C  

t1.txt 64*64
t2.txt 1024*1024
t3.txt 4096*4096
t4.txt 16384*16384
t5.txt 65536*65536


localproject:

compile: mpicc -I. -Wall -O3 localProject.c clcg4.c -o test -lm
 
run: mpirun -np x ./test keys m n k  
keys: a int number, if keys == 42, will generate matrix with all 1 for validation, else, it will generate random number





