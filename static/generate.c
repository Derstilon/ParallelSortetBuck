#include <omp.h>
#include <stdio.h>
int main()
{
    int SIZE_OF_TABLE = 10000;
    int AMOUNT_OF_THREADS = 4;
    int *array;
    array = malloc(SIZE_OF_TABLE * sizeof(int));
    double t0 = omp_get_wtime();
#pragma omp parallel num_threads(AMOUNT_OF_THREADS)
    {
#pragma omp single
        {
            printf(" Liczba wątków: %d \n", omp_get_num_threads());
            printf(" Jestem wątek nr %d \n ", omp_get_thread_num());
            printf("\n");
        }
#pragma omp for schedule(static, SIZE_OF_TABLE / AMOUNT_OF_THREADS)
        for (int i = 0; i < SIZE_OF_TABLE; i++)
        {
            array[i] = omp_get_thread_num();
        }
    }
    double t1 = omp_get_wtime();
    printf("\n");
    printf(" Czas wykonania: %f \n", t1 - t0);
}