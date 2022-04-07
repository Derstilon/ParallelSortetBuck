#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --mem=100M
#SBATCH --time=00:05:00
#SBATCH --partition=plgrid-short
#SBATCH --account=plgyaptide

# module add plgrid/tools/openmp

# gcc -Wall src/populate_array.c -o src/populate_array -fopenmp
gcc -Wall src/main.c src/populate_array.c src/bucketSort1.c src/bucketSort2.c src/bucketSort3.c -o src/main -fopenmp
# ./src/main 1 4 10000 10000 10
./src/main 2 4 10000 10000 10
# ./src/main 3 4 10000 10000 10

# for (( i=12; i>0; i-- ))
# do
#     for (( j=8; j<=8; j+=2 ))
#     do
#         for (( k=1; k<=10; k++))
#         do
#             mpiexec -n $i python3 main.py $((10**j))
#         done
#     done
# done