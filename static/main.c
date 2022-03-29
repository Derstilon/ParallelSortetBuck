#include <omp.h>
#include <stdio.h>
int main()
{
    int AMOUNT_OF_THREADS = 4;
    int *table;
    int SIZE_OF_TABLE = 10000;
    int SIZE_OF_CHUNKS = SIZE_OF_TABLE / AMOUNT_OF_THREADS;
    int SIZE_OF_ENDCHUNK = SIZE_OF_TABLE % SIZE_OF_CHUNKS;

    /*
     * Kod wykonywany s e k w e n c y j n i e
     */
    table = malloc(SIZE_OF_TABLE * sizeof(int));
    printf(" Liczba wątków: %d \n", omp_get_num_threads());
    printf(" Jestem wątek nr %d \n ", omp_get_thread_num());
    printf("\n");
    int k;
#pragma omp parallel for firstprivate(SIZE_OF_CHUNKS, SIZE_OF_ENDCHUNK, SIZE_OF_TABLE, AMOUNT_OF_THREADS) shared(table) num_threads(AMOUNT_OF_THREADS)
    for (k = 0; k < AMOUNT_OF_THREADS; k++)
    {
        int i = 0;
        if (omp_get_thread_num() == AMOUNT_OF_THREADS - 1)
        {
            for (i = 0; i < SIZE_OF_CHUNKS + SIZE_OF_ENDCHUNK; i++)
            {
                table[i] = omp_get_thread_num();
            }
        }
        else
        {
            for (i = 0; i < SIZE_OF_CHUNKS; i++)
            {
                table[i] = omp_get_thread_num();
            }
        }
    }
    int j = 0;
    for (j = 0; j < SIZE_OF_TABLE; j++)
    {
        printf("%d ", table[j]);
    }
    printf("\n");
}