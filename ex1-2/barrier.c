/**
 * CS2106 AY 20/21 Semester 1 - Lab 3
 *
 * This file contains function definitions. Your implementation should go in
 * this file.
 */
#include "semaphore.h"
#include "barrier.h"

// Initialise barrier here
void barrier_init ( barrier_t *barrier, int count ) {
    barrier->count = count;
    barrier->n = 0;
    sem_init(&barrier->mutex, 0, 1);
    sem_init(&barrier->turnstile1, 0, 0);
    sem_init(&barrier->turnstile2, 0, 0);
    // sem_init(&barrier->number_, 0, 1);
    // sem_init(&barrier->fizz_, 0, 0);
    // sem_init(&barrier->buzz_, 0, 0);
    // sem_init(&barrier->fizzbuzz_, 0, 0);
}

void phase1_barrier(barrier_t *barrier) {
  sem_wait(&barrier->mutex);
  if (++barrier->n == barrier->count) {
      sem_post(&barrier->turnstile1);
  }
  sem_post(&barrier->mutex);
  sem_wait(&barrier->turnstile1);
  sem_post(&(barrier->turnstile1));
}

void phase2_barrier(barrier_t *barrier)
{
  sem_wait(&barrier->mutex);
  if (--barrier->n == 0) {
      sem_post(&barrier->turnstile2);
  }
  sem_post(&barrier->mutex);
  sem_wait(&barrier->turnstile2);
  sem_wait(&barrier->turnstile2);
}

void barrier_wait ( barrier_t *barrier ) {
    // sem_wait(&barrier->mutex);
    // // barrier->n++;
    // sem_post(&barrier->mutex);
    // if (barrier->n == barrier-> count) {
    //     sem_post(&barrier->q);
    // }
    // sem_wait(&barrier->q);
    // sem_post(&barrier->q);
    phase1_barrier(barrier);
    phase2_barrier(barrier);
}

// Perform cleanup here if you need to
void barrier_destroy ( barrier_t *barrier ) {
    sem_destroy(&barrier->mutex);
    sem_destroy(&barrier->turnstile1);
    sem_destroy(&barrier->turnstile2);
}
