#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int SIZE_OF_TABLE = 10000;
    int AMOUNT_OF_THREADS = 4;
    int *array;
    int MAX_NUMBER = 10000;
    array = malloc(SIZE_OF_TABLE * sizeof(int));
    double t0 = omp_get_wtime();
    #pragma omp parallel num_threads(AMOUNT_OF_THREADS)
    {
        int i;
        unsigned int seed = 2137;
        #pragma omp for schedule(static, SIZE_OF_TABLE / AMOUNT_OF_THREADS) private(i)
        for (i = 0; i < SIZE_OF_TABLE; i++)
        {
            array[i] = rand_r(&seed)%MAX_NUMBER;
        }
    }
    double t1 = omp_get_wtime();
    printf("\n");
    printf(" Czas wykonania: %f \n", t1 - t0);

    int i;
    for (i = 0; i<SIZE_OF_TABLE; i++) {
        printf("%d \n", array[i]);
    }

    return 0;
}