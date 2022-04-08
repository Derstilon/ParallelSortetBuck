#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define BUCKET_BOTTOM(i,j)      (sorted_array + (i*number_of_ranges + j)*bucket_size)
#define BUCKET_TOP(i,j)         (sorted_array + (i*number_of_ranges + j)*bucket_size + bucket_fullness[i][j])
#define STRINGIFY(x)            #x
#define SCHEDULE_FOR(tasks)     _Pragma(STRINGIFY(omp for schedule(static, tasks / amount_of_threads) private(i, j, k)))
#define SCHEDULE_SINGLE         _Pragma("omp single private(i, j, k)")
#define SCHEDULE_TIME(tx)       SCHEDULE_SINGLE {tx = omp_get_wtime();}

void sortChunk(int*,int);

void bucketSort3(
    int amount_of_threads,
    int array_size,
    int *array,
    int *sorted_array,
    int bucket_size,
    int number_of_ranges,
    int bucket_range)
{
    // Initialize the buckets on sorted array
    sorted_array = malloc(number_of_ranges*bucket_size*amount_of_threads*sizeof(int));
    int i = 0, j = 0, k = 0;
    double t0=0, t1=0, t2=0, t3=0, t4=0, t5=0, t6=0, t7=0;
    int **bucket_fullness = malloc(amount_of_threads * sizeof(int *));
    for (k = 0; k < amount_of_threads; k++)
        bucket_fullness[k] = malloc(number_of_ranges * sizeof(int));
    #pragma omp parallel num_threads(amount_of_threads)
    {

        /***************** ASSIGN  BUCKETS *****************/
        SCHEDULE_TIME(t0);
        SCHEDULE_FOR(array_size)
        for (k = 0; k < array_size; k++){
            i = omp_get_thread_num();
            j = array[k]/bucket_range;
            BUCKET_TOP(i,j)[0] = array[k];
            bucket_fullness[i][j]++;
        }
        SCHEDULE_TIME(t1);

        /************* COLLAPSE THREAD BUCKETS *************/
        SCHEDULE_TIME(t2);
        SCHEDULE_FOR(number_of_ranges)
        for (j = 0; j < number_of_ranges; j++)
            for (i = 1; i < amount_of_threads; i++)
                for (k = 0; k < bucket_fullness[i][j]; k++){
                    BUCKET_TOP(0,j)[0] = BUCKET_BOTTOM(i,j)[k];
                    bucket_fullness[0][j]++;
                }
        SCHEDULE_TIME(t3);

        /**************** SORT BUCKETS TODO ****************/
        SCHEDULE_TIME(t4);
        SCHEDULE_FOR(number_of_ranges)
        for (j = 0; j < number_of_ranges; j++)
            sortChunk(BUCKET_BOTTOM(0,j), bucket_fullness[0][j]);
        SCHEDULE_TIME(t5);

        /************** COLLAPSE RESULT_ARRAY **************/
        SCHEDULE_TIME(t6);
        #pragma omp single private(i, j, k)
        {
            for (i = 1; i < number_of_ranges; i++)
                for (k = 0; k < bucket_fullness[0][i]; k++){
                    BUCKET_TOP(0,0)[0] = BUCKET_BOTTOM(0,i)[k];
                    bucket_fullness[0][0]++;
                }
        }
        SCHEDULE_TIME(t7);
        #pragma omp single private(i, j, k)
        {
            printf("\n");
            printf("Czas przypisania wartości do kubelkow: %e \n", t1 - t0);
            printf("Czas połączenia kubelkow:              %e \n", t3 - t2);
            printf("Czas sortowania kubelkow:              %e \n", t5 - t4);
            printf("Czas połączenia wynikow:               %e \n", t7 - t6);
        }
    }
}
