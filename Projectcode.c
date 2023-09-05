#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

sem_t sem1;
sem_t sem2;
sem_t sem3;
pthread_mutex_t mutex;

int limit=0, x=0, count=0, tchair=0, num, flag=0;

void *Patients(void *arg)
{
    sem_wait(&sem1);
    int num = (int*)arg;
    pthread_mutex_lock(&mutex);

    if (count >tchair)
    {
        printf("\nNo more chairs available. Patient %d is leaving from the DENTAL CLINIC.", num);
        pthread_mutex_unlock(&mutex);
        pthread_exit(num);
    }

    if (count!=0)
    {
        printf("Patient %d is waiting in the chair in the CLINIC.\n",num);
    }

    count++;
    pthread_mutex_unlock(&mutex);
    sem_wait(&sem2);
    flag++;
    sleep(1);
    if (flag==1)
    {
        printf("\n\nPatient %d wakes up the dentist\n",num);
    }
    x=num;
    sem_post(&sem3);
}

void *Dentist(void *arg)
{
    while (true)
    {
        if (limit)
        {
            printf("\nTHE DENTIST TERMINATES.\n");
            break;
        }
        sem_wait(&sem3);
        printf("\nThe dentist is treating patient '%d' \n", x);
        printf("Patient %d is treated successfully.\n", x);
        printf("Patient %d is leaving.\n", x);
        sem_post(&sem2);
    }
}

int main()
{
    pthread_t dentist, *arr_patients;

    int patient,chair;

    printf("\n\n\n************ DENTAL CLINIC PROBLEM ************\n\n\n");
    printf("Number of total patients: ");
    scanf("%d", &patient);
    printf("Number of total chairs: ");
    scanf("%d", &chair);
    printf("\n");

    tchair= chair;

    sem_init(&sem1, 0, patient);
    sem_init(&sem2, 0, 1);
    sem_init(&sem3, 0, 0);

    pthread_mutex_init(&mutex, 0);

    if (patient==0) {printf("Dentist is sleeping\n");}


    arr_patients=(pthread_t*)malloc(patient*sizeof(pthread_t));

    pthread_create(&dentist,NULL,Dentist,NULL);

    for (int i=0;i<patient;i++)
    {
        pthread_create(&arr_patients[i],NULL,Patients, (void*)i+1);
    }
    for (int i=0;i<patient;i++)
    {
        pthread_join(arr_patients[i], NULL);
    }
    limit=1;
    sem_post(&sem3);
    pthread_join(dentist, NULL);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem1);
    sem_destroy(&sem2);
    sem_destroy(&sem3);
}
