#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void populate_array(int, int, int *, int, unsigned int);

void bucketSort1(int, int, int *, int *, int, int, int);
void bucketSort2(int, int, int *, int *, int, int, int);
void bucketSort3(int, int, int *, int *, int, int, int);

int main(int argc, char **argv)
{
    int variant;
    if (argc > 1)
    {
        variant = atoi(argv[1]);
    }
    else
    {
        variant = 1;
    }
    int NUMBER_OF_THREADS = 4;
    int ARRAY_SIZE = 10000;
    int *array, *sorted_array;
    int MAX_NUMBER = 10000;
    int BUCKET_SIZE = ARRAY_SIZE / 2;
    int NUMBER_OF_RANGES = 10;
    int BUCKET_RANGE = MAX_NUMBER / NUMBER_OF_RANGES;
    array = malloc(ARRAY_SIZE * sizeof(int));
    populate_array(NUMBER_OF_THREADS, ARRAY_SIZE, array, MAX_NUMBER, 2137);
    switch (variant)
    {
    case 1:
        bucketSort1(NUMBER_OF_THREADS, ARRAY_SIZE, array, sorted_array, BUCKET_SIZE, NUMBER_OF_RANGES, BUCKET_RANGE);
        break;
    case 2:
        bucketSort2(NUMBER_OF_THREADS, ARRAY_SIZE, array, sorted_array, BUCKET_SIZE, NUMBER_OF_RANGES, BUCKET_RANGE);
        break;
    case 3:
        bucketSort3(NUMBER_OF_THREADS, ARRAY_SIZE, array, sorted_array, BUCKET_SIZE, NUMBER_OF_RANGES, BUCKET_RANGE);
        break;
    default:
        printf("Wrong variant");
    }
    free(array);
    free(sorted_array);

    return 0;
}