#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void populate_array(int amount_of_threads, int array_size, int *array, unsigned int max_value, unsigned int seed)
{
    #pragma omp parallel num_threads(amount_of_threads)
    {
        int i;
        unsigned int x = seed, y = max_value;
        #pragma omp for schedule(static, array_size / amount_of_threads) private(i,x)
        for (i = 0; i < array_size; i++)
            array[i] = rand_r(&x) % y;
    }
}