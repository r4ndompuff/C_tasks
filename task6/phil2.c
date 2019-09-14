#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>

/* Version without errors for some UNIX systems, for example OS X */

#define LEFT (i + N - 1) % N // Defining number of left philosopher
#define RIGHT (i + 1) % N // Defining number of right philosopher
#define WALKING 0 // Walking status
#define HUNGRY 1 // Waiting for forks status
#define EATING 2 // Eating status
#define MAX_TIME 5 // Max time for walking and eating

/* Solving method is descriped in Tanenbaum book and in internet :p */

int N; // Amount of philosophers
sem_t *mutex = NULL; // Critical expansion (For example, took and drop forks)
sem_t *eaters = NULL; // Pointer on database of N semaphors
int* state = NULL; // Pointer on database of N statuses of each philosopher

/* We don't really need to work with forks, if we can work with philosophers, it will make code much faster*/

/*-------------------------------------------------------Eating-----------------------------------------------*/
void eat(int i) {
    int time = rand() % MAX_TIME;
    printf("Philosopher #%d is eating...\n", i + 1);
    sleep(time);
    printf("Philosopher #%d stopped eating...\n", i + 1);
}

/*-------------------------------------------------------Walking-----------------------------------------------*/
void walk(int i) {
    int time = rand() % MAX_TIME;
    printf("Philosopher #%d is walking...\n", i + 1);
    sleep(time);
}

/*-------------------------------------------------------Testing-----------------------------------------------*/
/* Testing, if i-philosopher can start eating */
void test(int i) { // If his neighbors are not eating and philosopher is hungry then there is free forks for him 
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        state[i] = EATING;
        sem_post(&eaters[i]); // Philosopher starts eating, thats why we incrementing semaphor, because his neihbros can block him
    }
}

/*---------------------------------------------Trying_to_take_forks-----------------------------------------------*/
void take_forks(int i) {
    sem_wait(mutex); // Inserting to critical expansion, because with forks must work 1 philosopher
    state[i] = HUNGRY;
    test(i); // Trying to take forks
    sem_post(mutex); // Leaving critical expansion
    sem_wait(&eaters[i]); // If we can't start eating, then we are waiting for 'eaters'
}

/*----------------------------------------------Trying_to_put_forks-----------------------------------------------*/
void put_forks(int i) {
    sem_wait(mutex); // Inserting to critical expansion, because with forks must work 1 philosopher
    state[i] = WALKING;
    test(LEFT); // If left philosopher is hungry, then he can start eating
    test(RIGHT);
    sem_post(mutex); // Leaving critical expansion
}


/*---------------------------------------------Every_philospher_thread-----------------------------------------------*/
void* philosopher(void* arg) {
    int i = *((int*)arg);
    while (1 == 1) { // Infinity loop
        walk(i); // First he is walking
        take_forks(i); // He is trying to take forks
        eat(i); // Eating
        put_forks(i); // Putting his forks back
    }
}

/*---------------------------------------------Main-----------------------------------------------*/
int main(int argc, char* argv[]) {
    N = 5;
    mutex = (sem_t*)malloc(sizeof(sem_t));
    eaters = (sem_t*)calloc(N, sizeof(sem_t)); // N semaphores - philosophers
    state = (int*)calloc(N, sizeof(int)); // N args for philosophers status 
    memset(state, 0, N);
    srand(time(NULL));
    pthread_t *philosophers = (pthread_t*)malloc(N * sizeof(pthread_t)); // Making n threads - creating philosophers
    int i;
    int *t = (int *)malloc(sizeof(int));
    for (i = 0; i < N; i++) {
        *t = i;
        if (pthread_create(&philosophers[i], NULL, philosopher, t) != 0) { // Making threads
            fprintf(stderr, "Error by creating thread\n");
            return 2;
        }
        /* Usleep -> That's because parent-link can change his number (i) for the next link before intializating previous one. 
        That's mean that some links can use one number, and it is bad :( */
        usleep(100000); 
    }
    void* result;
    for (i = 0; i < N; i++) {
        if ((int)philosophers[i] != -1) {
            if (pthread_join(philosophers[i], &result) != 0) {
                fprintf(stderr, "Error by joining thread\n");
                return 3;
            }
        }
    }
    return 0;
}
