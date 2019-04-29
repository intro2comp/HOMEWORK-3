
#define _XOPEN_SOURCE 500
#define _REENTRANT
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "monitor.h"

pthread_mutex_t outlock;
int nsteps, maxsteps = 0; // number of steps to run this test

int eat_count[NTHREADS];  // number of steps for each thread

void trace(int i, char *s) {
  pthread_mutex_lock(&outlock);
  if (strcmp (s, "is Eating") == 0) eat_count [i]++;

  fprintf(stdout,"%d: %s\n",i,s);

  if (nsteps++ > maxsteps) {
    if (strcmp(s,"is Thinking") == 0) {
      pthread_mutex_unlock(&outlock);

      fprintf (stderr, "Thread done\n");

      pthread_exit(0);
    }
  }
  pthread_mutex_unlock(&outlock);
}

void * philosopher_body(void *arg) {
  int self = *(int *) arg;
  for (;;) {
     trace(self,"is Thinking");
     chopsticks_take(self);
     trace(self,"is Eating");
     chopsticks_put(self);
  }
}

int main() {
  int i;
  pthread_t th[NTHREADS]; // IDs of the philospher threads
  int       no[NTHREADS]; // corresponding table position numbers
  pthread_attr_t attr;

  for (i = 0; i < NTHREADS; i++) eat_count [i] = 0;

  pthread_mutex_init(&outlock, NULL);

  // initialize the object chopsticks
  chopsticks_init();

  fprintf(stdout,"Number of iterations to run: "); fflush(stdout);
  fscanf(stdin,"%d",&maxsteps);

  clock_t begin = clock(); // Begin execution timer

  pthread_attr_init (&attr);
  pthread_setconcurrency (4);
  pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);

  // Start up the philosopher threads
  for (i = 0; i < NTHREADS; i++) {
    no[i] = i;
    pthread_create(&th[i], NULL, philosopher_body, (int *) &no[i]);
  }

  // Wait for all the threads to shut down
  for (i = 0; i < NTHREADS; i++) pthread_join(th[i], NULL);

  printf("\n");

  for (i = 0; i < NTHREADS; i++) {
      fprintf (stderr, "Philospher %d ate %d times\n", i, eat_count [i]);
  }

  clock_t end = clock(); // End execution timer
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("\nExecution time: %f\n", time_spent);

  return 0;
}
