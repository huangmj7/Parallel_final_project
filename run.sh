#!/bin/bash

pwd="/gpfs/u/home/PCP8/PCP8hnhh/scratch/proj"

nrank=1
key=10


m1=108
m=$m1
#echo $m
srun --ntasks $nrank --overcommit -o ${nrank}_${m}.txt ${pwd}/main.xl $key $m $m $m


m=$(($m1 * 2))
#echo $m
srun --ntasks $nrank --overcommit -o ${nrank}_${m}.txt ${pwd}/main.xl $key $m $m $m

m=$(($m1 * 3))
#echo $m
srun --ntasks $nrank --overcommit -o ${nrank}_${m}.txt ${pwd}/main.xl $key $m $m $m


m=$(($m1 * 4))
#echo $m
srun --ntasks $nrank --overcommit -o ${nrank}_${m}.txt ${pwd}/main.xl $key $m $m $m

m=$(($m1 * 5))
#echo $m
srun --ntasks $nrank --overcommit -o ${nrank}_${m}.txt ${pwd}/main.xl $key $m $m $m








