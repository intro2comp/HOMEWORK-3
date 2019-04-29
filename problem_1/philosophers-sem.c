
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define NTHREADS 5
#define THINKING 1
#define HUNGRY 2
#define EATING 3
#define LEFT (i + 4) % NTHREADS
#define RIGHT (i + 1) % NTHREADS

int state[NTHREADS];
int phil[NTHREADS] = { 0, 1, 2, 3, 4 };
int nsteps, maxsteps = 0;
int eat_count[NTHREADS];

sem_t mutex;
sem_t S[NTHREADS];

// Sleep makes to program take too long to test large iterations but has accurate results.
// Sleeps were commented out and replaced with sleep(0) instead to have a chance to see execution time.


void test(int i)
{
    if (state[i] == HUNGRY
        && state[LEFT] != EATING
        && state[RIGHT] != EATING) {

        state[i] = EATING;

        // sleep(2);
        sleep(0);

        printf("%d: is Eating\n", i);
        eat_count [i]++;

        sem_post(&S[i]);
    }
}

// take up chopsticks
void take_fork(int i)
{

    sem_wait(&mutex);

    // state that hungry
    state[i] = HUNGRY;

    printf("%d: is Hungry\n", i);

    // eat if neighbours are not eating
    test(i);

    sem_post(&mutex);

    // if unable to eat wait to be signalled
    sem_wait(&S[i]);

    // sleep(1);
    sleep(0);
}

// put down chopsticks
void put_fork(int i)
{

    sem_wait(&mutex);

    // state that thinking
    state[i] = THINKING;

    printf("%d: is Thinking\n", i);

    test(LEFT);
    test(RIGHT);

    if (nsteps++ > maxsteps){
        sem_post(&mutex);
        fprintf (stderr, "Thread done\n");
        pthread_exit(0);
    }

    sem_post(&mutex);
}

void* philospher(void* num)
{
    while (1) {
        int* i = num;
        // sleep(1);
        sleep(0);

        take_fork(*i);
        sleep(0);

        put_fork(*i);
    }
}

int main()
{
    int i;
    pthread_t thread_id[NTHREADS];

    // initialize the semaphores
    sem_init(&mutex, 0, 1);

    fprintf(stdout,"Number of iterations to run: "); fflush(stdout);
    fscanf(stdin,"%d",&maxsteps);

    clock_t begin = clock(); /* Begin execution timer */

    for (i = 0; i < NTHREADS; i++)
        sem_init(&S[i], 0, 0);

    /* Start up the philosopher threads */
    for (i = 0; i < NTHREADS; i++) {
        pthread_create(&thread_id[i], NULL, philospher, &phil[i]);
    }

    /* Wait for all the threads to shut down */
    for (i = 0; i < NTHREADS; i++) pthread_join(thread_id[i], NULL);

    printf("\n");

    for (i = 0; i < NTHREADS; i++) {
        fprintf (stderr, "Philospher %d ate %d times\n", i, eat_count [i]);
    }

    clock_t end = clock(); /* End execution timer */
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\nExecution time: %f\n", time_spent);

    return 0;
}
