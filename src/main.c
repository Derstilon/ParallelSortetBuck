#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define min(a, b) (a > b ? b : a)

const int REPEATS = 10;

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

void populate_array(int, int, int *, int, unsigned int);

void bucketSort2(int, int, int *, int *, int, int, int, double *);
void bucketSort3(int, int, int *, int *, int, int, int, double *);

int main(int argc, char **argv)
{
    int variant, NUMBER_OF_THREADS, ARRAY_SIZE, MAX_NUMBER, NUMBER_OF_RANGES;

    if (argc > 5)
    {
        variant = atoi(argv[1]);
        NUMBER_OF_THREADS = atoi(argv[2]);
        ARRAY_SIZE = atoi(argv[3]);
        MAX_NUMBER = atoi(argv[4]);
        NUMBER_OF_RANGES = atoi(argv[5]);
    }
    else
    {
        variant = 2;
        NUMBER_OF_THREADS = 4;
        ARRAY_SIZE = 10000;
        MAX_NUMBER = 10000;
        NUMBER_OF_RANGES = 10;
    }
    int BUCKET_RANGE = (MAX_NUMBER / NUMBER_OF_RANGES) + ((MAX_NUMBER % NUMBER_OF_RANGES) > 0 ? 1 : 0);
    int BUCKET_SIZE = min(ARRAY_SIZE, (ARRAY_SIZE / NUMBER_OF_RANGES) * 2);
    int *array, *sorted_array;

    array = malloc(ARRAY_SIZE * sizeof(int));
    populate_array(NUMBER_OF_THREADS, ARRAY_SIZE, array, MAX_NUMBER, 2137);

    double **times = malloc(REPEATS * sizeof(double *));
    int i, times_number;
    for (i = 0; i < REPEATS; i++)
    {
        switch (variant)
        {
        case 2:
            times_number = 4;
            times[i] = malloc(times_number*sizeof(double));
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
        }
        free(sorted_array);
    }

    print_average(times, times_number);

    for (i = 0; i < REPEATS; i++)
        free(times[i]);
    free(times);
    free(array);

    return 0;
}