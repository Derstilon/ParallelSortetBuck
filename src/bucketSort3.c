#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUCKET_FULLNESS(th, rg) \
    bucket_fullness[th][rg]

#define BUCKET_POINTER(th, rg) \
    (sorted_array + (th * number_of_ranges + rg) * bucket_size)

#define BUCKET_VALUE(th, rg, offset) \
    (BUCKET_POINTER(th, rg)[offset])

#define BUCKET_TOP(th, rg) \
    BUCKET_VALUE(th, rg, bucket_fullness[th][rg])

#define STRINGIFY(x) #x

#define SCHEDULE_FOR_PRAGMA(block) \
    _Pragma(STRINGIFY(omp for schedule(dynamic, block) private(el, th, rg, fl)))

#define SCHEDULE_FOR(var, iterator)                                                        \
    SCHEDULE_FOR_PRAGMA(var / amount_of_threads + ((var % amount_of_threads) > 0 ? 1 : 0)) \
    for (iterator = 0; iterator < var; iterator++)

#define SCHEDULE_SINGLE \
    _Pragma("omp single private(el, th, rg, fl)")

#define SCHEDULE_TIME(tx) \
    SCHEDULE_SINGLE { tx = omp_get_wtime(); }

#define SCHEDULE_MAIN \
    _Pragma("omp parallel num_threads(amount_of_threads) shared(bucket_fullness, sorted_array)")

void quickSortChunk(int *, int);

void bucketSort3(
    int amount_of_threads,
    int array_size,
    int *array,
    int *sorted_array,
    int bucket_size,
    int number_of_ranges,
    int bucket_range,
    double *times)
{
    // Initialize the buckets on sorted array
    sorted_array = (int *)malloc(number_of_ranges * bucket_size * amount_of_threads * sizeof(int));
    memset(sorted_array, 0, number_of_ranges * bucket_size * amount_of_threads * sizeof(int));
    int th = 0, rg = 0, fl = 0, el = 0;
    double t0 = 0, t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;
    int **bucket_fullness = (int **)malloc(amount_of_threads * sizeof(int *));
    for (th = 0; th < amount_of_threads; th++)
    {
        bucket_fullness[th] = (int *)malloc(number_of_ranges * sizeof(int));
        memset(bucket_fullness[th], 0, number_of_ranges * sizeof(int));
    }
    SCHEDULE_MAIN
    {

        /***************** ASSIGN  BUCKETS *****************/
        SCHEDULE_TIME(t0);
        SCHEDULE_FOR(array_size, el)
        {
            th = omp_get_thread_num();
            rg = array[el] / bucket_range;
            BUCKET_TOP(th, rg) = array[el];
            BUCKET_FULLNESS(th, rg)
            ++;
        }
        SCHEDULE_TIME(t1);

        /************* COLLAPSE THREAD BUCKETS *************/
        SCHEDULE_TIME(t2);
        SCHEDULE_FOR(number_of_ranges, rg)
        {
            for (th = 1; th < amount_of_threads; th++)
                for (fl = 0; fl < BUCKET_FULLNESS(th, rg); fl++)
                {
                    BUCKET_TOP(0, rg) = BUCKET_VALUE(th, rg, fl);
                    BUCKET_FULLNESS(0, rg)
                    ++;
                }
        }
        SCHEDULE_TIME(t3);

        // /**************** SORT BUCKETS TODO ****************/
        SCHEDULE_TIME(t4);
        SCHEDULE_FOR(number_of_ranges, rg)
        {
            quickSortChunk(BUCKET_POINTER(0, rg), BUCKET_FULLNESS(0, rg));
        }
        SCHEDULE_TIME(t5);

        /************** COLLAPSE RESULT_ARRAY **************/
        SCHEDULE_TIME(t6);
        SCHEDULE_SINGLE
        {
            for (rg = 1; rg < number_of_ranges; rg++)
                for (fl = 0; fl < BUCKET_FULLNESS(0, rg); fl++)
                {
                    BUCKET_TOP(0, 0) = BUCKET_VALUE(0, rg, fl);
                    BUCKET_FULLNESS(0, 0)
                    ++;
                }
        }
        SCHEDULE_TIME(t7);
        SCHEDULE_SINGLE
        {
            // printf("Czas przypisania warto??ci do kubelkow: %e \n", t1 - t0);
            // printf("Czas po????czenia kubelkow:              %e \n", t3 - t2);
            // printf("Czas sortowania kubelkow:              %e \n", t5 - t4);
            // printf("Czas po????czenia wynikow:               %e \n", t7 - t6);
            // printf("Czas ca??kowity:                        %e \n", t7 - t0);
            times[0] = t1 - t0;
            times[1] = t3 - t2;
            times[2] = t5 - t4;
            times[3] = t7 - t6;
            times[4] = t7 - t0;
        }
    }
    for (th = 0; th < amount_of_threads; th++)
        free(bucket_fullness[th]);
    free(bucket_fullness);
}