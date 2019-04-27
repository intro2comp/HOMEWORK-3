#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define TAXI_COUNT 5
#define CUSTOMER_COUNT 2

bool QUIT = false;

pthread_t *customers, *taxis;  // Thread arrays
sem_t sem_customer, sem_taxi;  // Counting semaphores

void *taxi(void *arg)
{
    int index = *((int *) arg);

    while (!QUIT) {
        sem_post(&sem_taxi);
        printf("Taxi %d ready\n", index);
        sem_wait(&sem_customer);
        printf("Customer found\n");
        // Taxi picks up customer...
        // done();
    }

    free(arg);

    pthread_exit(0);
}

void *customer(void *arg)
{
    int index = *((int *) arg);

    while (!QUIT) {
        sem_post(&sem_customer);
        printf("Customer %d ready\n", index);
        sem_wait(&sem_taxi);
        // Customer boards taxi...
        // done();
    }

    free(arg);

    pthread_exit(0);
}

int main()
{
    // Initialize counting semaphores
    sem_init(&sem_customer, 0, 0);
    sem_init(&sem_taxi, 0, 0);

    customers = malloc(CUSTOMER_COUNT * sizeof(pthread_t));
    taxis = malloc(TAXI_COUNT * sizeof(pthread_t));

    for (int i = 0; i < TAXI_COUNT; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&(taxis[i]), NULL, taxi, arg);
    }

    printf("Taxis created\n");

    for (int i = 0; i < CUSTOMER_COUNT; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&(customers[i]), NULL, customer, arg);
    }

    for (int i = 0; i < TAXI_COUNT; i++) {
        pthread_join(taxis[i], NULL);
    }

    for (int i = 0; i < CUSTOMER_COUNT; i++) {
        pthread_join(customers[i], NULL);
    }

    printf("Good night!\n");

    return 0;
}
