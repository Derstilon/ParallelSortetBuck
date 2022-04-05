void bucketSort3(int number_of_threads, int array_size, int *array, int bucket_size, int number_of_buckets, int bucket_range)
{
    int *sorted_array = malloc(array_size * sizeof(int));
    int *buckets = malloc(number_of_buckets * bucket_size * sizeof(int));
    double t0 = omp_get_wtime();
    int i = 0;
    int j = 0;
    int **bucket_tops = malloc(number_of_buckets * sizeof(int *));
    int *bucket_fullness = malloc(number_of_buckets * sizeof(int));
    for (i = 0; i < number_of_buckets; i++)
    {
        bucket_tops[i] = buckets + i * bucket_size;
        bucket_fullness[i] = 0;
    }

    // to be parallelized
    // private variables:
    // bucket_tops
    // bucket_fullness
    // buckets
    int number_of_thread = 0;
    for (i = 0; i < array_size; i++)
    {
        for (j = 0; j < number_of_buckets; j++)
        {
            if (array[i] > j && array[i] < j * bucket_range)
            {
                *bucket_tops[j] = array[i];
                bucket_fullness++;
            }
        }
    }
    double t1 = omp_get_wtime();
    printf("\n");
    printf(" Czas wykonania: %f \n", t1 - t0);
    for (i = 0; i < number_of_buckets; i++)
    {
        printf("%d \n", bucket_fullness[i]);
    }
}