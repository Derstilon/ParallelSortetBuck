#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --mem=100M
#SBATCH --time=00:05:00
#SBATCH --partition=plgrid-short
#SBATCH --account=plgyaptide
#SBATCH --output="output.out"

# module add plgrid/tools/openmp

SCRATCH_DIRECTORY=/net/scratch/people/${USER}/${SLURM_JOBID}
mkdir -p ${SCRATCH_DIRECTORY}
cd ${SCRATCH_DIRECTORY}

cp ${SLURM_SUBMIT_DIR}/src/main.c ${SCRATCH_DIRECTORY}
cp ${SLURM_SUBMIT_DIR}/src/populate_array.c ${SCRATCH_DIRECTORY}
cp ${SLURM_SUBMIT_DIR}/src/sortChunk.c ${SCRATCH_DIRECTORY}
cp ${SLURM_SUBMIT_DIR}/src/bucketSort1.c ${SCRATCH_DIRECTORY}
cp ${SLURM_SUBMIT_DIR}/src/bucketSort2.c ${SCRATCH_DIRECTORY}
cp ${SLURM_SUBMIT_DIR}/src/bucketSort3.c ${SCRATCH_DIRECTORY}


gcc -Wall main.c populate_array.c sortChunk.c bucketSort1.c bucketSort2.c bucketSort3.c -o main -fopenmp
# ./src/main 1 4 10000 10000 10
# ./src/main 2 4 10000 10000 10
./main 3 4 10000 10000 10

cd ${SLURM_SUBMIT_DIR}
rm -rf ${SCRATCH_DIRECTORY}

# Finish the script
exit 0