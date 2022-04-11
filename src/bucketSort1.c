#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void sortChunk(int *, int);

void bucketSort1(int number_of_threads, int array_size, int *array, int *sorted_array, int bucket_size, int number_of_buckets, int bucket_range)
{
    sorted_array = malloc(number_of_buckets * bucket_size * sizeof(int));
    int i = 0, j = 0, k = 0;
    double t0 = 0, t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;
    int buckets_per_thread = number_of_buckets / number_of_threads;

    int **bucket_fullness = malloc(number_of_buckets * sizeof(int *));
    int *bucket_length = malloc(number_of_buckets * sizeof(int));

    for (k = 0; k < number_of_buckets; k++)
        bucket_fullness[k] = &sorted_array[k * bucket_size];

    for (i = 0; i < bucket_size * number_of_buckets; i++)
    {
        sorted_array[i] = 0;
    }
    for (i = 0; i < number_of_buckets; i++)
    {
        bucket_length[i] = 0;
    }

#pragma omp parallel private(i) num_threads(number_of_threads)
    {
        int thread_no = omp_get_thread_num();

        // place in correct bucket
        for (i = 0; i < array_size; i++)
        {
            if (array[i] >= thread_no * bucket_range * number_of_buckets / number_of_threads && array[i] < (thread_no + 1) * bucket_range * number_of_buckets / number_of_threads)
            {
                const int bucket_no = (array[i] - (thread_no * bucket_range * number_of_buckets / number_of_threads)) / bucket_range;
                *bucket_fullness[thread_no * number_of_buckets / number_of_threads + bucket_no] = array[i];
                bucket_fullness[thread_no * number_of_buckets / number_of_threads + bucket_no]++;
                bucket_length[thread_no * number_of_buckets / number_of_threads + bucket_no]++;
            }
        }

        // sort buckets
        for (i = thread_no * number_of_buckets / number_of_threads; i < (thread_no + 1) * number_of_buckets / number_of_threads; i++)
        {
            sortChunk(&sorted_array[i * bucket_size], bucket_length[i]);
        }
    }
    int index = 0;
    for (i = 0; i < number_of_buckets; i++)
    {
        for (j = i * bucket_size; j < i * bucket_size + bucket_length[i]; j++)
        {
            array[index++] = sorted_array[j];
        }
    }
    for (i = 0; i < array_size; i++)
    {
        printf("%d ", array[i]);
    }
}

int main()
{
    int number_of_threads = 3;
    int array_size = 15;
    int array[15] = {14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    int *sorted_array;
    int bucket_size = 10;
    int number_of_buckets = 3;
    int bucket_range = 5;

    bucketSort1(number_of_threads, array_size, array, sorted_array, bucket_size, number_of_buckets, bucket_range);

    return 0;
}