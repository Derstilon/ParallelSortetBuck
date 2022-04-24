#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void sortChunk(unsigned int *, int);
void quickSortChunk(unsigned int *, int);

void bucketSort2(
    int number_of_threads,
    int array_size,
    unsigned int *array,
    unsigned int *sorted_array,
    int bucket_size,
    int number_of_buckets,
    int bucket_range,
    double *time_results)
{
    int BASIC_CHUNK_SIZE = array_size / number_of_threads;
    // int *chunks_sizes = malloc(number_of_threads * sizeof(int));
    int i;
    double timer;
    // for (i = 0; i < number_of_threads; i++)
    // {
    //     chunks_sizes[i] = BASIC_CHUNK_SIZE;
    // }
    // chunks_sizes[number_of_threads - 1] = array_size % number_of_threads;
    // int *table;
    // table = malloc(array_size * sizeof(int));

    unsigned int **buckets = malloc(number_of_buckets * sizeof(unsigned int *));
    unsigned int *bucket_indexes = malloc(number_of_buckets * sizeof(unsigned int));
    omp_lock_t *bucket_locks = malloc(number_of_buckets * sizeof(omp_lock_t));

    for (i = 0; i < number_of_buckets; i++)
    {
        buckets[i] = malloc(bucket_size * sizeof(unsigned int));
        bucket_indexes[i] = 0;
        omp_init_lock(&(bucket_locks[i]));
    }
    #pragma omp parallel num_threads(number_of_threads) default(shared)
    {
        timer = omp_get_wtime();
        #pragma omp for schedule(static, BASIC_CHUNK_SIZE) private(i)
        for(i = 0; i < array_size; i++) {
            int elem = array[i];
            int bucket_idx = elem / bucket_range <= number_of_buckets ? bucket_idx = elem / bucket_range : number_of_buckets - 1;
            omp_set_lock(&(bucket_locks[bucket_idx]));
            buckets[bucket_idx][bucket_indexes[bucket_idx]] = elem;
            bucket_indexes[bucket_idx] = bucket_indexes[bucket_idx] + 1;
            omp_unset_lock(&(bucket_locks[bucket_idx]));
        }
        #pragma omp barrier
        #pragma omp single 
        {
            time_results[0] = omp_get_wtime() - timer;
            timer = omp_get_wtime();
        }
        // SORT
        #pragma omp for schedule(dynamic) private(i)
        for(i = number_of_buckets - 1; i >= 0; i--) {
            quickSortChunk(buckets[i], bucket_indexes[i]);
        }
        #pragma omp barrier
        #pragma omp single 
        {
            time_results[1] = omp_get_wtime() - timer;
            for(i = 0; i < number_of_buckets - 1; i++) {
                bucket_indexes[i+1] += bucket_indexes[i];
            }
            timer = omp_get_wtime();
        }
        // MERGE
        #pragma omp for schedule(dynamic) private(i)
        for(i = number_of_buckets - 1; i >= 0; i--) {
            int array_start_idx, array_end_idx;
            array_start_idx = i > 0 ? bucket_indexes[i-1] : 0;
            array_end_idx = bucket_indexes[i];
            int j, k = 0;
            for(j = array_start_idx; j <= array_end_idx; j++){
                sorted_array[j] = buckets[i][k++];
            }
        }
        #pragma omp single
        {
            time_results[2] = omp_get_wtime() - timer;
        }

    }


    for (i = 0; i < number_of_buckets; i++)
    {
        omp_destroy_lock(&(bucket_locks[i]));
        free(buckets[i]);
    }
    free(buckets);
    free(bucket_indexes);
    free(bucket_locks);
// #pragma omp for schedule(static, BASIC_CHUNK_SIZE) private(i, x)
//     for (i = omp_get_thread_num() * BASIC_CHUNK_SIZE; i < min(array_size, (omp_get_thread_num() + 1) * BASIC_CHUNK_SIZE); i++)
//     {
//         x = array[i];
//         int bucket_idx = number_of_buckets - 1;
//         if (x / bucket_range <= number_of_buckets)
//         {
//             bucket_idx = x / bucket_range;
//         }
//         buckets[bucket_idx][bucket_indexes[bucket_idx]] = x;
//         bucket_indexes[bucket_idx] = bucket_indexes[bucket_idx] + 1;
//     }
    // for (i = 0; i < number_of_buckets; i++)
    // {
    //     for (j = 0; j < bucket_indexes[i]; j++)
    //     {
    //         printf(" %d ", buckets[i][j]);
    //     }
    //     printf("\n");
    // }
}
