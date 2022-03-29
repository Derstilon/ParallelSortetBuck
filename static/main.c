#include <omp.h>
#include <stdio.h>
int main()
{
    int *table;
    int SIZE_OF_TABLE = 10000;
    int AMOUNT_OF_THREADS = 4;
    int SIZE_OF_CHUNKS = SIZE_OF_TABLE / AMOUNT_OF_THREADS;
    int SIZE_OF_ENDCHUNK = SIZE_OF_TABLE % AMOUNT_OF_THREADS;

    /*
     * Kod wykonywany s e k w e n c y j n i e
     */
    table = (int *)malloc(sizeof(int) * SIZE_OF_TABLE);
    printf(" Liczba wątków: %d \n", omp_get_num_threads());
    printf(" Jestem wątek nr %d \n ", omp_get_thread_num());
    printf("\n");
#pragma omp parallel firstprivate(var1, var2, SIZE_OF_TABLE, AMOUNT_OF_THREADS) shared(table)
    {
        if (omp_get_thread_num() == AMOUNT_OF_THREADS - 1)
        {
            for (int i = 0; i < SIZE_OF_CHUNKS + SIZE_OF_ENDCHUNK; i++)
            {
                table[i] = omp_get_thread_num();
            }
        }
        else
        {
            for (int i = 0; i < SIZE_OF_CHUNKS; i++)
            {
                table[i] = omp_get_thread_num();
            }
        }
    }
    for (int i = 0; i < SIZE_OF_TABLE; i++)
    {
        printf("%d ", table[i]);
    }
    printf("\n");
}