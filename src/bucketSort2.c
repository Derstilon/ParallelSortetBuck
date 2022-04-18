#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define min(a, b) ((a) < (b) ? (a) : (b))

void bucketSort2(
    int number_of_threads,
    int array_size,
    unsigned int *array,
    unsigned int *sorted_array,
    int bucket_size,
    int number_of_buckets,
    int bucket_range)
{
    int BASIC_CHUNK_SIZE = array_size / number_of_threads;
    int *chunks_sizes = malloc(number_of_threads * sizeof(int));
    int i, j, x;
    for (i = 0; i < number_of_threads; i++)
    {
        chunks_sizes[i] = BASIC_CHUNK_SIZE;
    }
    chunks_sizes[number_of_threads - 1] = array_size % number_of_threads;
    // int *table;
    // table = malloc(array_size * sizeof(int));

    int **buckets = malloc(number_of_buckets * sizeof(int *));
    int *bucket_indexes = malloc(number_of_buckets * sizeof(int));
    for (i = 0; i < number_of_buckets; i++)
    {
        buckets[i] = malloc(bucket_size * sizeof(int));
        bucket_indexes[i] = 0;
    }

#pragma omp for schedule(static, BASIC_CHUNK_SIZE) private(i, x)
    for (i = omp_get_thread_num() * BASIC_CHUNK_SIZE; i < min(array_size, (omp_get_thread_num() + 1) * BASIC_CHUNK_SIZE); i++)
    {
        x = array[i];
        int bucket_idx = number_of_buckets - 1;
        if (x / bucket_range <= number_of_buckets)
        {
            bucket_idx = x / bucket_range;
        }
        buckets[bucket_idx][bucket_indexes[bucket_idx]] = x;
        bucket_indexes[bucket_idx] = bucket_indexes[bucket_idx] + 1;
    }
    for (i = 0; i < number_of_buckets; i++)
    {
        for (j = 0; j < bucket_indexes[i]; j++)
        {
            printf(" %d ", buckets[i][j]);
        }
        printf("\n");
    }
}
