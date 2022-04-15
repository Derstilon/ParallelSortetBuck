#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --mem=6GB
#SBATCH --time=00:59:59
#SBATCH --partition=plgrid-testing
#SBATCH --account=plgyaptide
#SBATCH --output="output.out"

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

# sort variants
for (( v=3; v<=3; v++ ))
do
    # number of threads
    for (( t=1; t<=1; t++ ))
    do
        # number of ranges
        for (( r=100000; r<=100000; r*=2))
        do
            echo "Variant: $v, Threads: $t, Ranges: $r"
            # ./main $v $t 5000 32767 $r
            ./main $v $t 1000000 1000000 $r 2137
            echo
        done
    done
done

cd ${SLURM_SUBMIT_DIR}
rm -rf ${SCRATCH_DIRECTORY}

# Finish the script
exit 0