// === Updated dining.c ===
#include "dining.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define EATING_TIME 2
#define THINKING_TIME 1

pthread_mutex_t forks[NUM_PHILOSOPHERS];
pthread_t philosophers[NUM_PHILOSOPHERS];
pthread_mutex_t table_mutex = PTHREAD_MUTEX_INITIALIZER;

void *philosopher(void *num) {
    int id = *(int *)num;
    int left_fork = id;
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;
    int first_fork = (id % 2 == 0) ? left_fork : right_fork;
    int second_fork = (id % 2 == 0) ? right_fork : left_fork;

    for (int i = 0; i < 3; i++) {
        printf("Philosopher %d is thinking...\n", id);
        sleep(THINKING_TIME);

        pthread_mutex_lock(&forks[first_fork]);
        printf("Philosopher %d picked up fork %d\n", id, first_fork);

        if (pthread_mutex_trylock(&forks[second_fork])) {
            pthread_mutex_unlock(&forks[first_fork]);
            printf("Philosopher %d couldn't pick up fork %d, released fork %d\n", id, second_fork, first_fork);
            continue;
        }

        printf("Philosopher %d picked up fork %d and is eating\n", id, second_fork);
        sleep(EATING_TIME);

        pthread_mutex_unlock(&forks[first_fork]);
        pthread_mutex_unlock(&forks[second_fork]);
        printf("Philosopher %d put down forks %d and %d\n", id, first_fork, second_fork);
    }

    printf("Philosopher %d has left the table.\n", id);
    return NULL;
}

void start_dining_philosophers() {
    int ids[NUM_PHILOSOPHERS];

    printf("\nDining Philosophers Simulation\n");
    printf("Each philosopher will think and eat 3 times using deadlock prevention.\n\n");

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
        ids[i] = i;
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    printf("\nDining Philosophers simulation completed successfully.\n");
}
