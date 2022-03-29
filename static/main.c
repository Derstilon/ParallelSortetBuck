#include <omp.h>
#include <stdio.h>
main()
{
    int *table;
    int SIZE_OF_TABLE = 10000;

    /*
     * Kod wykonywany s e k w e n c y j n i e
     */
    table = (int *)malloc(sizeof(int) * SIZE_OF_TABLE);
    printf(" Liczba wątków: %d \n", omp_get_num_threads());
    printf(" Jestem wątek nr %d \n ", omp_get_thread_num());
    printf("\n");
/**/
#pragma omp parallel private(var1, var2) shared(var3)
    /* Początek
        o b s z a r u r ó wn ole g ł ego.Tw o r z e ni e g r u py wą t k ów.Okre ś l a n i e
            z a s i ę gu zmie n nych */
    {
        /* Kod wykonywany r ó wn ole g ł e(p r z e z w s z y s t k i e wą t k i) */
        printf(" Liczba wą tk ów: %d \n", omp_get_num_threads());
        printf(" Jestem wą tek nr %d \n", omp_get_thread_num());

    } /* W s z y s t k i e wą t k i s i ę s y n c h r o n i z u j ą */
    /**/
    printf("\n ");
    /* K o n ty n u a cj a s e k w e n c yj n e g o wykonywania kodu */
    printf(" Liczba wątków: %d \n", omp_get_num_threads());
    printf(" Jestem wątek nr %d\n", omp_get_thread_num());
}