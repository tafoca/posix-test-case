//gcc -pthread -o montecarlo montecarlo.c && ./montecarlo
/**  P r o g r a m m e  p o u r  a p p r o x i m e r  la  v a l e u r  de  pi
 * a  l ’ a i d e  d ’ u n e  m e t h o d e  M o n t e  C a r l o 
 *   n b L a n c e r s  n b T h r e a d s*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// nb pts dans le cercle de rayon 1
void *nbDansCercleSeq(void *nbLancers)
{
    //  On  e f f e c t u e  l e s  l a n c e r s .
    long nb = 0;
    for (long i = 0; i < (long)nbLancers; i++)
    {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0)
        {
            nb += 1;
        }
    }
    //  On  r e t o u r n e  le  r e s u l t a t  au  p a r e n t .
    pthread_exit((void *)nb);
}

double evaluerPi(long nbLancers, long nbThreads)
{ /*  On  i n i t i a l i s e  la  s t r u c t u r e 
 d ’ a t t r i b u t  p o u r  le  t h r e a d  ( s t a n d a r d ).*/
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    //  On  cree  les  threads
    pthread_t threads[nbThreads];
    for (long i = 0; i < nbThreads; i++)
    {                                                                                         /*  On  p a s s e  l'u n i q u e  a r g u m e n t  e n t i e r
          p a r  l ’ i n t e r m e d i a i r e  d ’ un  c a s t .*/
        pthread_create(&threads[i], &attr, nbDansCercleSeq, (void *)(nbLancers / nbThreads)); //equit distribution lancer a chq threat
    }
    //  On  o b t i e n t  l e s  r e s u l t a t s  d e s  t h r e a d s .
    long nbTotalDansCercle = 0;
    for (long i = 0; i < nbThreads; i++)
    {
        long res;
        pthread_join(threads[i], (void *)&res);
        nbTotalDansCercle += res;
    }
    return 4.0 * nbTotalDansCercle / nbLancers;
}

//  P r o g r a m m e  p r i n c i p a l
int main(int argc, char *argv[])
{
    assert(argc >= 3);
    long nbLancers = atoi(argv[1]);
    assert(nbLancers > 0);
    long nbThreads = atoi(argv[2]);
    assert(nbThreads > 0);
    double pi = evaluerPi(nbLancers, nbThreads);
    printf("pi = %f\n", pi);
    return 0;
}
//gcc -pthread -o montecarlo montecarlo.c && ./montecarlo 100 20