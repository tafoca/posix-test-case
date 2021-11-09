//sum-vectors/mat-sum-val/matsumvalue.c
/**
 * @file matsumvalue.c
 * @author Tabueu laurent (laurent.tabueu@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-09
 * 
 * @copyright Copyright (c) 2021
 * 
 * on Unix system
 * gcc -pthread -o matsumvalue matsumvalue.c && ./matsumvalue  size  numWorkers
 * 
 * ex gcc -pthread -o matsumvalue matsumvalue.c && ./matsumvalue  100 4
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SHARED 1
#define MAXSIZE 10000                                                      /*  m a x i m u m  m a t r i x  s i z e  */
#define MAXWORKERS 4 /*  m a x i m u m  n u m b e r  of  w o r k e r s  */ //
pthread_mutex_t barrier;                                                   /*  m u t e x  l o c k  f o r  t h e  b a r r i e r  */
pthread_cond_t go;                                                         /*  c o n d i t i o n  v a r i a b l e  f o r  l e a v i n g  */
int numWorkers;
int numArrived = 0; /*  n u m b e r  w h o  h a v e  a r r i v e d  */
                    /*  a  r e u s a b l e  c o u n t e r  b a r r i e r  */
void Barrier()
{
    pthread_mutex_lock(&barrier);
    numArrived++;
    if (numArrived == numWorkers)
    {
        numArrived = 0;
        pthread_cond_broadcast(&go);
    }
    else
    {
        pthread_cond_wait(&go, &barrier);
    }
    pthread_mutex_unlock(&barrier);
}

int size, stripSize; /*  a s s u m e  s i z e  is  m u l t i p l e  of  n u m W o r k e r s  */
int sums[MAXWORKERS];
int matrix[MAXSIZE][MAXSIZE];
void *Worker(void *); //
/*  r e a d  c o m m a n d  line ,  i n i t i a l i z e ,  a n d  c r e a t e  t h r e a d s  */
int main(int argc, char *argv[])
{
    long i, j;
    pthread_attr_t attr;
    pthread_t workerid[MAXWORKERS]; /*  s e t  g l o b a l  t h r e a d  a t t r i b u t e s  */
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM); /*  i n i t i a l i z e  m u t e x  a n d  c o n d i t i o n  v a r i a b l e  */
    pthread_mutex_init(&barrier, NULL);
    pthread_cond_init(&go, NULL); /*  r e a d  c o m m a n d  l i n e  */
    size = atoi(argv[1]);
    numWorkers = atoi(argv[2]);
    stripSize = size / numWorkers; /*  i n i t i a l i z e  t h e  m a t r i x  */
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            matrix[i][j] = 1;
        }
    } //
      /*  c r e a t e  t h e  w o r k e r s ,  t h e n  w a i t  */
    for (i = 0; i < numWorkers; i++)
    {
        pthread_create(&workerid[i], &attr, Worker, (void *)i);
    }
    for (long i = 0; i < numWorkers; i++)
    {
        pthread_join(workerid[i], NULL);
    }
    pthread_exit(NULL);
} 

  /*  E a c h  w o r k e r  s u m s  t h e  v a l u e s  in  o n e  s t r i p  of 
 t h e  m a t r i x .A f t e r  a  b a r r i e r ,  w o r k e r ( 0 )  
 c o m p u t e s  a n d  p r i n t s  t h e  t o t a l  */
void *Worker(void *arg)
{
    long myid = (long)arg;
    long total, i, j, first, last;
    printf("worker %ld (pthread  id %ld) has  started\n", myid, (long)pthread_self()); /*  d e t e r m i n e  f i r s t  a n d  l a s t  r o w s  of  my  s t r i p  */
    first = myid * stripSize;
    last = first + stripSize - 1; /*  s u m  v a l u e s  in  my  s t r i p  */
    total = 0;
    for (i = first; i <= last; i++)
    {
        for (j = 0; j < size; j++)
        {
            total += matrix[i][j];
        }
    }
    sums[myid] = total;
    Barrier();
    if (myid == 0)
    {
        total = 0;
        for (i = 0; i < numWorkers; i++)
        {
            total += sums[i];
        }
        printf("the  total is %ld\n", total);
    }
    return NULL;
}
