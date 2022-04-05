#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --mem=1K
#SBATCH --time=00:05:00
#SBATCH --partition=plgrid-short
#SBATCH --account=plgyaptide

module add plgrid/tools/openmp

gcc -Wall static/generate.c -o static/generate -fopenmp
./static/generate

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