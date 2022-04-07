#define BUCKET_BOTTOM(i,j) (sorted_array + (i*number_of_ranges + j)*bucket_size)
#define BUCKET_TOP(i,j)    (BUCKET_BOTTOM(i,j) + bucket_fullness[i][j])
#define WRITE_TIME(tx) _Pragma("omp single") {tx = omp_get_wtime();}
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

    #pragma omp parallel num_threads(amount_of_threads)
    {
        int i = 0, j = 0, k = 0;
        double t0, t1, t2, t3, t4, t5, t6, t7;
        int **bucket_fullness = malloc(amount_of_threads * sizeof(int *));

        #pragma omp for schedule(static, amount_of_threads) private(i)
        for (i = 0; i < amount_of_threads; i++)
            bucket_fullness[i] = malloc(number_of_ranges * sizeof(int));


        /***************** ASSIGN  BUCKETS *****************/
        WRITE_TIME(t0);
        #pragma omp for schedule(static, array_size / amount_of_threads) private(i, j, k)
        for (k = 0; k < array_size; k++){
            i = omp_get_thread_num();
            j = k/bucket_range;
            BUCKET_TOP(i,j)[0] = array[k];
            bucket_fullness[i][j]++;
        }
        WRITE_TIME(t1);

        // /************* COLLAPSE THREAD BUCKETS *************/
        // WRITE_TIME(t2);
        // #pragma omp for schedule(static, number_of_ranges / amount_of_threads) private(i, j, k)
        // for (j = 0; j < number_of_ranges; j++)
        //     for (i = 1; i < amount_of_threads; i++)
        //         for (k = 0; k < bucket_fullness[i][j]; k++){
        //             BUCKET_TOP(0,j)[0] = BUCKET_BOTTOM(i,j)[k];
        //             bucket_fullness[0][j]++;
        //         }
        // WRITE_TIME(t3);

        // /**************** SORT BUCKETS TODO ****************/
        // WRITE_TIME(t4);
        // #pragma omp for schedule(static, amount_of_threads) private(i, j)
        // for (j = 0; j < number_of_ranges; j++)
        //     sortChunk(BUCKET_BOTTOM(0,j), bucket_fullness[0][j]);
        // WRITE_TIME(t5);

        // /************** COLLAPSE RESULT_ARRAY **************/
        // WRITE_TIME(t6);
        // #pragma omp single private(i, j, k)
        // {
        //     for (i = 1; i < number_of_ranges; i++)
        //         for (k = 0; k < bucket_fullness[0][i]; k++){
        //             BUCKET_TOP(0,0)[0] = BUCKET_BOTTOM(0,i)[k];
        //             bucket_fullness[0][0]++;
        //         }
        // }
        // WRITE_TIME(t7);
        #pragma omp single private(k)
        {
            printf("\n");
            printf(" Czas przypisania wartości do kubelkow: %f \n", t1 - t0);
            printf(" Czas połączenia kubelkow: %f \n", t3 - t2);
            printf(" Czas sortowania kubelkow: %f \n", t5 - t4);
            printf(" Czas połączenia wynikow: %f \n", t7 - t6);

            for (k = 0; k < array_size; k++)
                printf("%d \n", sorted_array[k]);
        }
        for(i = 0; i < amount_of_threads; i++)
            free(bucket_fullness[i]);
        free(bucket_fullness);
    }
}
