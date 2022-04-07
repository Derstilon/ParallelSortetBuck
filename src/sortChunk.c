#define SWAP(i,j) {int tmp = chunk[i]; chunk[i] = chunk[j]; chunk[j] = tmp;}

void sortChunk(
    int *chunk,
    int chunk_size)
{
    int i, j;
    for (i = 0; i < chunk_size; i++)
        for (j = i; j > 0 && chunk[j] < chunk[j-1]; j--)
            SWAP(j, j-1);
}