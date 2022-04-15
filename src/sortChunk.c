#define SWAP(i, j)           \
    {                        \
        int tmp = chunk[i];  \
        chunk[i] = chunk[j]; \
        chunk[j] = tmp;      \
    }

void sortChunk(
    unsigned int *chunk,
    int chunk_size)
{
    int i, j;
    for (i = 0; i < chunk_size; i++)
        for (j = i; j > 0 && chunk[j] < chunk[j - 1]; j--)
            SWAP(j, j - 1);
}

void quickSortChunk(
    unsigned int *chunk,
    int chunk_size)
{
    int i, j, pivot;
    if (chunk_size <= 1)
        return;
    pivot = chunk[chunk_size / 2];
    for (i = 0, j = chunk_size - 1;; i++, j--)
    {
        while (chunk[i] < pivot)
            i++;
        while (chunk[j] > pivot)
            j--;
        if (i >= j)
            break;
        SWAP(i, j);
    }
    quickSortChunk(chunk, i);
    quickSortChunk(chunk + i, chunk_size - i);
}

void insertSortChunk(
    unsigned int *chunk,
    int chunk_size)
{
    int i, j;
    for (i = 1; i < chunk_size; i++)
    {
        for (j = i; j > 0 && chunk[j] < chunk[j - 1]; j--)
            SWAP(j, j - 1);
    }
}