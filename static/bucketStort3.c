void bucketSort3(int number_of_threads, int array_size, int *array, int bucket_size, int number_of_buckets, int bucket_range)
{
    int *sorted_array = malloc(array_size * sizeof(int));
    int *buckets = malloc(number_of_buckets * bucket_size * sizeof(int));
    double t0 = omp_get_wtime();
#pragma omp parallel num_threads(AMOUNT_OF_THREADS) private(*buckets)
    {
    }
}