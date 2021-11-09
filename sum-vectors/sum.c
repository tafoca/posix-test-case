/*  P r o g r a m m e  q u i  g e n e r e  d e u x  t ab l e a u x  p u i s  q u i  c a l c u l el e u r  so mm e , 
 en  p a r a l l e l e:
g c c  sum.c  - o  so m me  -std=c99  -lp t h r e a d s omme - t a b l e a u x  n b E l e m e n t s  n b T h r e a d s*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

//  S t r u c t u r e  p o u r  le  t r a n s f e r t  d e s  a r g u m e n t s  a u x  t h r e a d s .
typedef struct
{
    double *a;
    double *b;
    double *c;
    long nbElements;
} Args; //
//  La  f o n c t i o n  e x ec u t e e  p a r  l e s  t h r e a d s .
void *somme_tableaux_seq(void *arg)
{                             //  On  i d e n t i f i e  l e s  " v r a i s "  a r g u m e n t s .
    Args *args = (Args *)arg; //  E v i t e  l e s  c a s t  s u b s e q u e n t s .
    double *a = args->a;
    double *b = args->b;
    double *c = args->c;
    long nbElements = args->nbElements;
    //  On  f a i t  la  s o m m e  d e s  e l e m e n t s  du  b l o c  ( de  la  t r a n c h e ).
    for (long i = 0; i < nbElements; i++)
    {
        c[i] = a[i] + b[i];
    }
    //  A u c u n  r e s u l t a t  a  r e t o u r n e r .
    pthread_exit(NULL);
} //
//  La  f o n c t i o n  p r i n c i p a l e .
void somme_tableaux_par(double a[], double b[], double c[], long nbElements, long nbThreads)
{
    //  On  i n i t i a l i s e  l e s  a t t r i b u t s  d e s  t h r e a d s .
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    //  On  l a n c e  l e s  t h r e a d s :  d i s t r i b u t i o n  p a r  b l o c s .
    pthread_t threads[nbThreads];
    Args args[nbThreads];
    long nbParThread = nbElements / nbThreads;
    for (long i = 0; i < nbThreads; i++)
    { //  On  p a s s e  d i r e c t e m e n t  l ’ a d r e s s e  de  d e b u t  du  b l o c  a  t r a i t e r .
        args[i].a = a + i * nbParThread;
        args[i].b = b + i * nbParThread;
        args[i].c = c + i * nbParThread;
        args[i].nbElements = nbParThread;
        pthread_create(&threads[i], &attr, somme_tableaux_seq, &args[i]);
    }
    //  On  a t t e n d  q u e  l e s  t h r e a d s  se  t e r m i n e n t .
    for (int i = 0; i < nbThreads; i++)
    {
        pthread_join(threads[i], NULL);
    }
}
//  Le  p r o g r a m m e  p r i n c i p a l .
int main(int argc, char *argv[])
{
    //  On  l i t  l e s  a r g u m e n t s .
    assert(argc >= 3);
    long nbElements = atoi(argv[1]);
    assert(nbElements > 0);
    long nbThreads = atoi(argv[2]);
    assert(nbThreads > 0);
    //  On  s ’ a s s u r e  q u e  c h a q u e  t h r e a d  a u r a  le  m e m e  n o m b r e  d ’ e l e m e n t s  a  t r a i t e r .
    assert(nbElements % nbThreads == 0);
    //  On  a l l o u e  et  i n i a l i s e  l e s  t a b l e a u x .
    double *a = (double *)malloc(nbElements * sizeof(double));
    double *b = (double *)malloc(nbElements * sizeof(double));
    double *c = (double *)malloc(nbElements * sizeof(double));
    for (long i = 0; i < nbElements; i++)
    {
        a[i] = 1.0;
        b[i] = 10.0;
    }
    // Start measuring time
    time_t begin, end;
    time(&begin);
    //  On  e f f e c t u e  la  s om m e ,  en  p a r a l l e l e .
    somme_tableaux_par(a, b, c, nbElements, nbThreads);
    // Stop measuring time and calculate the elapsed time
    time(&end);
    time_t elapsed = end - begin;
    printf("Time measured: %ld seconds.\n", elapsed);

    //  On  v e r i f i e  le  r e s u l t a t .
    for (long i = 0; i < nbElements; i++)
    {
        //printf("%ld =>\t %.2f=%.2f \n", i, c[i], ((a[i] + b[i])));
        assert(c[i] == (a[i] + b[i]));
    }
    return 0;
}
//gcc -pthread -o sum sum.c && ./sum 100 10
