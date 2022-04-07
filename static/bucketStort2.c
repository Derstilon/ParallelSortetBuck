#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void bucketSort2(int number_of_threads, int array_size, int *array, int bucket_size, int number_of_buckets, int bucket_range) {
    int* sorted_array = malloc(array_size * sizeof(int));

    int SIZE_OF_CHUNKS = array_size / number_of_threads;
    int SIZE_OF_ENDCHUNK = array_size % number_of_threads;
    // int *table;
    // table = malloc(array_size * sizeof(int));
    
    int **buckets = malloc(number_of_buckets * sizeof(*int));
    int *bucket_indexes = malloc(number_of_buckets * sizeof(int))
    int i, j;
    for(i=0; i<number_of_buckets; i++) {
        buckets[i] = malloc(bucket_size * sizeof(int));
        bucket_indexes[i] = 0;
    }

    // #pragma omp parallel num_threads(number_of_threads)
    // {
    //     #pragma omp for schedule(static, SIZE_OF_TABLE / AMOUNT_OF_THREADS) private(i)
    for(i = 0; i < array_size; i++) {
        x = array[i];
        int bucket_idx = number_of_buckets - 1;
        if(x/bucket_range <= number_of_buckets) {
            bucket_idx = x/bucket_range;
        }
        buckets[bucket_idx][bucket_indexes[bucket_idx]] = x;
        bucket_indexes[bucket_idx] = bucket_indexes[bucket_idx] + 1;
    }
    for(i = 0; i < number_of_buckets; i++) {
        for(j = 0; j < bucket_indexes[i]; j++) {
            printf(" %d ", buckets[i][j]);
        }
        printf("\n")
    }
}
