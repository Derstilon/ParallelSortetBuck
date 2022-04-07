#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void populate_array(int amount_of_threads, int array_size, int *array, int max_value, unsigned int seed)
{
#pragma omp parallel num_threads(amount_of_threads)
    {
        int i;
#pragma omp for schedule(static, array_size / amount_of_threads) private(i)
        for (i = 0; i < array_size; i++)
        {
            array[i] = rand_r(&seed) % max_value;
        }
    }

    return array;
}