/**
 * @file ex2-1.c
 * @author Tabueu Laurent
 * @brief 
 * @version 0.1
 * @date 2021-11-09
 * 
 * @copyright Copyright (c) 2021
 * 
 * description:
 * Le programme présente deux employés en compétition pour le titre d'"employé du jour", 
 * et la gloire qui l'accompagne. Pour simuler cela à un rythme rapide, le programme emploie 3 threads :
 * un qui promeut Lora au titre d'employé du jour", 
 * un qui promeut Laurent à cette situation, et 
 * un troisième thread qui s'assure que le contenu de l'employé du jour est cohérent 
 * (c'est-à-dire qu'il contient exactement les données d'un employé).
 */
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define NUM_EMPLOYEES 2
/* size of each array.
*/
/* global mutex for our program. assignment initializes it */
pthread_mutex_t a_mutex = PTHREAD_MUTEX_INITIALIZER;
struct employee
{
    int number;
    int id;
    char first_name[20];
    char last_name[30];
    char department[30];
    int room_number;
};
/* global variable - our employees array, with 2 employees */
struct employee employees[] = {
    {1, 12345678, "laurent", "Tabueu", "Accounting", 101},
    {2, 87654321, "lora", "Fotso", "Programmers", 202}};
struct employee employee_of_the_day; /* global variable - employee of the day. */

/* function to copy one employee struct into another */
void copy_employee(struct employee *from, struct employee *to)
{
    int rc; /* contain mutex lock/unlock results */
    /* lock the mutex, to assure exclusive access to 'a' and 'b'. */
    rc = pthread_mutex_lock(&a_mutex);
    to->number = from->number;
    to->id = from->id;
    strcpy(to->first_name, from->first_name);
    strcpy(to->last_name, from->last_name);
    strcpy(to->department, from->department);
    to->room_number = from->room_number;
    /* unlock mutex */
    rc = pthread_mutex_unlock(&a_mutex);
}
/* function to be executed by the variable setting threads thread */
void *do_loop(void *data)
{
    int my_num = *((int *)data); /* thread identifying number*/

    while (1)
    {
        /* set employee of the day to be the one with number 'my_num'. */
        copy_employee(&employees[my_num - 1], &employee_of_the_day);
    }
}

int main(int argc, char *argv[])
{
    int i;
    int thr_id1;
    int thr_id2;
    pthread_t p_thread1;
    pthread_t p_thread2;
    int num1 = 1;
    int num2 = 2;
    struct employee eotd;
    struct employee *worker;

    /* initialize employee of the day to first 1. */
    copy_employee(&employees[0], &employee_of_the_day);
    /* create a new thread that will execute 'do_loop()' with '1'*/
    thr_id1 = pthread_create(&p_thread1, NULL, do_loop, (void *)&num1);
    /* create a second thread that will execute 'do_loop()' with '2'*/
    thr_id2 = pthread_create(&p_thread2, NULL, do_loop, (void *)&num2);

    /* run a loop that verifies integrity of 'employee of the day' many , many many times.....*/
    for (i = 0; i < 60000; i++)
    {
        /* save contents of 'employee of the day' to local 'worker'. */
        copy_employee(&employee_of_the_day, &eotd);
        worker = &employees[eotd.number - 1];
        /* compare employees */
        if (eotd.id != worker->id)
        {
            printf("mismatching 'id' , %d != %d (loop '%d')\n",
                   eotd.id, worker->id, i);
            exit(0);
        }
        if (strcmp(eotd.first_name, worker->first_name) != 0)
        {
            printf("mismatching 'first_name' , %s != %s (loop '%d')\n",
                   eotd.first_name, worker->first_name, i);
            exit(0);
        }
        if (strcmp(eotd.last_name, worker->last_name) != 0)
        {
            printf("mismatching 'last_name' , %s != %s (loop '%d')\n",
                   eotd.last_name, worker->last_name, i);
            exit(0);
        }
        if (strcmp(eotd.department, worker->department) != 0)
        {
            printf("mismatching 'department' , %s != %s (loop '%d')\n",
                   eotd.department, worker->department, i);
            exit(0);
        }
        if (eotd.room_number != worker->room_number)
        {
            printf("mismatching 'room_number' , %d != %d (loop '%d')\n",
                   eotd.room_number, worker->room_number, i);
            exit(0);
        }
    }
    printf("Very nice, employees contents was always consistent\n");
    return 0;
}
//gcc -pthread -o ex2_1 ex2-1.c && ./ex2_1