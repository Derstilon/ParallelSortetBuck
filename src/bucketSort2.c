#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void sortChunk(int *, int);

void bucketSort2(
    int number_of_threads,
    int array_size,
    int *array,
    int *sorted_array,
    int bucket_size,
    int number_of_buckets,
    int bucket_range,
    double *time_results)
{
    sorted_array = malloc(array_size * sizeof(int));

    int BASIC_CHUNK_SIZE = array_size / number_of_threads;
    double timer;

    int **buckets = malloc(number_of_buckets * sizeof(int *));
    int *bucket_indexes = malloc(number_of_buckets * sizeof(int));
    omp_lock_t *bucket_locks = malloc(number_of_buckets * sizeof(omp_lock_t));

    int i;
    for (i = 0; i < number_of_buckets; i++)
    {
        buckets[i] = malloc(bucket_size * sizeof(int));
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
            sortChunk(buckets[i], bucket_indexes[i]);
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
    // printf("\n\n");
    // printf("%e\n", time_results[0]);
    // printf("%e\n", time_results[1]);
    // printf("%e\n", time_results[2]);
}
