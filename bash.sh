#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=00:05:00
#SBATCH --partition=plgrid-short
#SBATCH --account=plgyaptide

module add plgrid/tools/openmp

g++ -Wall static/main_stolen.cpp -o static/main_stolen -fopenmp
./static/main_stolen

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