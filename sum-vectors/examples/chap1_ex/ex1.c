/**
 * @file ex1.c
 * @author Tabueu laurent
 * @brief 
 * @version 0.1
 * @date 2021-11-09
 * 
 * @copyright Copyright (c) 2021
 * gcc -pthread -o ex1 ex1.c && ./ex1
 * Note that the main program is also a thread, so it executes the do_loop() function in parallel to the thread it
creates
 */
#include <stdio.h>
#include <pthread.h>

/* function to be executed by the new thread */
void *do_loop(void *data)
{
    int i;                   /* counter, to print numbers */
    int j;                   /* counter, for delay*/
    int me = *((int *)data); /* thread identifying number */
    for (i = 0; i < 10; i++)
    {
        for (j = 0; j < 500000; j++) /* delay loop */
            ;
        printf("'%d' - Got ' %d'\n", me, i);
    }
    /* terminate the thread */
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int thr_id; /* thread ID for the newly created thread */
    pthread_t p_thread; /* thread's structure*/
    int a = 1; /* thread 1 identifying number*/
    int b = 2; /* thread 2 identifying number*/
    /* create a new thread that will execute 'do_loop()' */
    thr_id = pthread_create(&p_thread, NULL, do_loop, (void *)&a);
    /* run 'do_loop()' in the main thread as well */
    do_loop((void *)&b);
    /* NOT REACHED */
    return 0;
}