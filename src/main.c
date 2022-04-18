#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define min(a, b) (a > b ? b : a)

const int REPEATS = 50;

void print_average(double **array, int times_number)
{
    int i, j;
    for (i = 1; i < REPEATS; i++)
    {
        for (j = 0; j < times_number; j++)
        {
            array[0][j] += array[i][j];
        }
    }
    for (j = 0; j < times_number; j++)
    {
        array[0][j] /= REPEATS;
        printf("%e\n", array[0][j]);
    }
}

void populate_array(int, int, unsigned int *, unsigned int, unsigned int);

void bucketSort1(int, int, unsigned int *, unsigned int *, int, int, int, double *);
void bucketSort2(int, int, unsigned int *, unsigned int *, int, int, int, double *);
void bucketSort3(int, int, unsigned int *, unsigned int *, int, int, int, double *);

int main(int argc, char **argv)
{
    int VARIANT, NUMBER_OF_THREADS, ARRAY_SIZE, NUMBER_OF_RANGES, SEED, BUCKET_RANGE, BUCKET_SIZE;
    unsigned int MAX_NUMBER;
    unsigned int *array, *sorted_array;
    double **times;
    int i, times_number;
    char *ptr;
    if (argc >= 6)
    {
        VARIANT = strtol(argv[1], &ptr, 10);
        NUMBER_OF_THREADS = strtol(argv[2], &ptr, 10);
        ARRAY_SIZE = strtol(argv[3], &ptr, 10);
        MAX_NUMBER = strtoul(argv[4], &ptr, 10);
        NUMBER_OF_RANGES = strtol(argv[5], &ptr, 10);
        SEED = strtol(argv[6], &ptr, 10);
    }
    else
    {
        VARIANT = 2;
        NUMBER_OF_THREADS = 4;
        ARRAY_SIZE = 10000;
        MAX_NUMBER = 10000;
        NUMBER_OF_RANGES = 10;
        SEED = 2137;
    }
    BUCKET_RANGE = (MAX_NUMBER / NUMBER_OF_RANGES) + ((MAX_NUMBER % NUMBER_OF_RANGES) > 0 ? 1 : 0);
    BUCKET_SIZE = min(ARRAY_SIZE, (ARRAY_SIZE / NUMBER_OF_RANGES) * 3 / 2);

    times = malloc(REPEATS * sizeof(double *));
    array = malloc(ARRAY_SIZE * sizeof(unsigned int));
    populate_array(NUMBER_OF_THREADS, ARRAY_SIZE, array, MAX_NUMBER, SEED);

    switch (VARIANT)
    {
    case 1:
        sorted_array = malloc(NUMBER_OF_RANGES * BUCKET_SIZE * sizeof(unsigned int));
        break;
    case 2:
        sorted_array = malloc(ARRAY_SIZE * sizeof(unsigned int));
        break;
    case 3:
        sorted_array = (unsigned int *)malloc(NUMBER_OF_RANGES * min(BUCKET_SIZE, BUCKET_SIZE * 8 / NUMBER_OF_THREADS) * NUMBER_OF_THREADS * sizeof(unsigned int));
        break;
    }
    for (i = 0; i < REPEATS; i++)
    {
        switch (VARIANT)
        {
        case 1:
            times_number = 3;
            times[i] = malloc(times_number * sizeof(double));
            bucketSort1(NUMBER_OF_THREADS, ARRAY_SIZE, array, sorted_array, BUCKET_SIZE, NUMBER_OF_RANGES, BUCKET_RANGE, times[i]);
            break;
        case 2:
            times_number = 4;
            times[i] = malloc(times_number * sizeof(double));
            double timer = omp_get_wtime();
            bucketSort2(NUMBER_OF_THREADS, ARRAY_SIZE, array, sorted_array, BUCKET_SIZE, NUMBER_OF_RANGES, BUCKET_RANGE, times[i]);
            times[i][3] = omp_get_wtime() - timer;
            break;
        case 3:
            times_number = 5;
            times[i] = malloc(times_number * sizeof(double));
            bucketSort3(NUMBER_OF_THREADS, ARRAY_SIZE, array, sorted_array, BUCKET_SIZE, NUMBER_OF_RANGES, BUCKET_RANGE, times[i]);
            break;
        default:
            printf("Wrong variant\n");
            return 0;
        }
    }

    print_average(times, times_number);

    for (i = 0; i < REPEATS; i++)
        free(times[i]);
    free(times);
    free(array);

    return 0;
}