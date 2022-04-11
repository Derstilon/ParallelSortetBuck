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
    int bucket_range)
{
    sorted_array = malloc(array_size * sizeof(int));

    int BASIC_CHUNK_SIZE = array_size / number_of_threads;
    // int *chunks_sizes = malloc(number_of_threads * sizeof(int));
    double timer, bucketing_time, sorting_time, merging_time;
    // for (i = 0; i < number_of_threads; i++)
    // {
    //     chunks_sizes[i] = BASIC_CHUNK_SIZE;
    // }
    // chunks_sizes[number_of_threads - 1] = array_size % number_of_threads;
    // int *table;
    // table = malloc(array_size * sizeof(int));

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
        // BUCKETING
        // #pragma omp for schedule(dynamic) private(i)
        // for (i = omp_get_thread_num() * BASIC_CHUNK_SIZE; i < min(array_size, (omp_get_thread_num() + 1) * BASIC_CHUNK_SIZE); i++)
        #pragma omp for schedule(static, BASIC_CHUNK_SIZE) private(i)
        for(i = 0; i < array_size; i++) {
            int element = array[i];
            int bucket_idx = number_of_buckets - 1;
            if (element / bucket_range <= number_of_buckets)
            {
                bucket_idx = element / bucket_range;
            }
            omp_set_lock(&(bucket_locks[bucket_idx]));
            buckets[bucket_idx][bucket_indexes[bucket_idx]] = element;
            bucket_indexes[bucket_idx] = bucket_indexes[bucket_idx] + 1;
            omp_unset_lock(&(bucket_locks[bucket_idx]));
        }
        #pragma omp single {
            bucketing_time = omp_get_wtime() - timer;
            timer = omp_get_wtime();
        }
        // SORT
        #pragma omp for schedule(dynamic) private(i)
        for(i = number_of_buckets - 1; i >= 0; i--) {
            sortChunk(buckets[i], bucket_indexes[i]);
        }
        #pragma omp single {
            sorting_time = omp_get_wtime() - timer;
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
            int k = 0;
            for(j = array_start_idx; j <= array_end_idx; j++){
                sorted_array[j] = buckets[i][k++];
            }
        }
        #pragma omp single {
            bucketing_time = omp_get_wtime() - timer;
        }

    }


    for (i = 0; i < number_of_buckets; i++)
    {
        omp_destroy_lock(&(bucket_locks[i]))
        for (j = 0; j < bucket_indexes[i]; j++)
        {
            printf(" %d ", buckets[i][j]);
        }
        printf("\n");
    }
}
