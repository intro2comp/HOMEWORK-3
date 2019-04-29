#ifndef CHOPSTICKS_H
#define CHOPSTICKS_H

#define NTHREADS 5 // The number of philosophers

extern void chopsticks_init();
extern void chopsticks_take(int i);
extern void chopsticks_put(int i);
extern void chopsticks_finalize();
extern void chopsticks_emergency_stop();

#endif
