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
problem_size=$((111000000))
seed=$((2137))
# sort variants
for (( v=3; v<=3; v++ ))
do
    # number of threads
    for (( t=12; t<=12; t++ ))
    do
        bucket_size=$(echo "sqrt($problem_size)" | bc -l | cut -d. -f1 )
        echo "Calculated optimal bucket size: $bucket_size"
        # number of ranges
        for (( r=256; r>=1; r/=2))
        do
            test_bucket_size=$(echo "$bucket_size/$r" | bc -l | cut -d. -f1 )
            echo "Variant: $v, Threads: $t, Ranges: $test_bucket_size"
            ./main $v $t $problem_size $problem_size $test_bucket_size $seed
            echo
        done
        for (( r=2; r<=10000; r*=2))
        do
            test_bucket_size=$(echo "$bucket_size/$r" | bc -l | cut -d. -f1 )
            echo "Variant: $v, Threads: $t, Ranges: $test_bucket_size"
            ./main $v $t $problem_size $problem_size $test_bucket_size $seed
            echo
        done
    done
done

cd ${SLURM_SUBMIT_DIR}
rm -rf ${SCRATCH_DIRECTORY}

# Finish the script
exit 0