#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void populate_array(int amount_of_threads, int array_size, int *array, int max_value, unsigned int seed);

// int *generate_array()
// {
//     int SIZE_OF_TABLE = 10000;
//     int AMOUNT_OF_THREADS = 4;
//     int *array;
//     int MAX_NUMBER = 10000;
//     array = malloc(SIZE_OF_TABLE * sizeof(int));
// #pragma omp parallel num_threads(AMOUNT_OF_THREADS)
//     {
//         int i;
//         unsigned int seed = 2137;
// #pragma omp for schedule(static, SIZE_OF_TABLE / AMOUNT_OF_THREADS) private(i)
//         for (i = 0; i < SIZE_OF_TABLE; i++)
//         {
//             array[i] = rand_r(&seed) % MAX_NUMBER;
//         }
//     }

//     return array;
// }

void bucketSort2(int number_of_threads, int array_size, int *array, int bucket_size, int number_of_buckets, int bucket_range)
{
    int *sorted_array = malloc(array_size * sizeof(int));

    int SIZE_OF_CHUNKS = array_size / number_of_threads;
    int SIZE_OF_ENDCHUNK = array_size % number_of_threads;

    int **buckets = malloc(number_of_buckets * sizeof(int *));
    int *bucket_indexes = malloc(number_of_buckets * sizeof(int));
    int i, j;
    for (i = 0; i < number_of_buckets; i++)
    {
        buckets[i] = malloc(bucket_size * sizeof(int));
        bucket_indexes[i] = 0;
    }

    for (i = 0; i < array_size; i++)
    {
        int x = array[i];
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
        printf(" Bucket %d, range: (%d, %d) \n", i, i * bucket_range, (i + 1) * bucket_range);
        for (j = 0; j < bucket_indexes[i]; j++)
        {
            printf(" %d ", buckets[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    int NUMBER_OF_THREADS = 4;
    int SIZE_OF_TABLE = 10000;
    int *array;
    int MAX_NUMBER = 10000;
    array = malloc(SIZE_OF_TABLE * sizeof(int));
    populate_array(NUMBER_OF_THREADS, SIZE_OF_TABLE, array, MAX_NUMBER, 2137);
    bucketSort2(4, 10000, array, 10000, 10, 1000);

    return 0;
}