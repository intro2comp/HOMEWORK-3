#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "queue.h"

#define TAXI_COUNT 5
#define CUSTOMER_COUNT 10

bool QUIT = false;

pthread_t *customers, *taxis;  // Thread arrays
sem_t sem_customer, sem_taxi;  // Counting semaphores

pthread_mutex_t lock_taxis;
pthread_cond_t taxis_available, taxis_used;

Queue *customer_queue, *taxi_queue;

void *taxi(void *arg)
{
    int *index = (int *) arg;

    while (!QUIT) {
        pthread_mutex_lock(&lock_taxis);  // Gain access to the taxi queue
        while (queue_is_full(taxi_queue)) {
            printf("Queue is full!\n");
            pthread_cond_wait(&taxis_used, &lock_taxis);
        }
        queue_push(taxi_queue, *index);    // Push taxi index onto queue
        printf("Taxi %d (%p) added to the queue\n", *index, arg);
        pthread_mutex_unlock(&lock_taxis);
        pthread_cond_signal(&taxis_available);
        sem_wait(&sem_customer);
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
        pthread_mutex_lock(&lock_taxis);
        while (queue_is_empty(taxi_queue)) {
            printf("Queue is empty!\n");
            pthread_cond_wait(&taxis_available, &lock_taxis);
        }
        printf("Customer %d boarding taxi %d\n", index, queue_pop(taxi_queue));
        pthread_mutex_unlock(&lock_taxis);
        pthread_cond_signal(&taxis_used);
        sleep(rand() % 10);
        // Customer boards taxi...
        // done();
    }

    free(arg);

    pthread_exit(0);
}

int main()
{
    srand(time(NULL));

    // Initialize counting semaphores
    sem_init(&sem_customer, 0, 0);
    sem_init(&sem_taxi, 0, 0);

    // Initialize locking semaphores
    pthread_mutex_init(&lock_taxis, NULL);

    pthread_cond_init(&taxis_available, NULL);
    pthread_cond_init(&taxis_used, NULL);

    customers = malloc(CUSTOMER_COUNT * sizeof(pthread_t));
    taxis = malloc(TAXI_COUNT * sizeof(pthread_t));

    int *ids = malloc(TAXI_COUNT * sizeof(int));

    for (int j = 0; j < TAXI_COUNT; j++) {
        ids[j] = j;
    }

    for (int j = 0; j < TAXI_COUNT; j++) {
        printf("Index: %d\n", ids[j]);
    }

    taxi_queue = queue_init(TAXI_COUNT);

    for (int i = 0; i < TAXI_COUNT; i++) {
        printf("Creating taxi %d with arg %p\n", i, (void *) &ids[i]);
        pthread_create(&(taxis[i]), NULL, taxi, (void *) &ids[i]);
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

    free(ids);
    free(customers);
    free(taxis);

    printf("Good night!\n");

    return 0;
}
