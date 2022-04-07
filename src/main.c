#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void populate_array(int amount_of_threads, int array_size, int *array, int max_value, unsigned int seed);
void bucketSort2(int number_of_threads, int array_size, int *array, int bucket_size, int number_of_buckets, int bucket_range);

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