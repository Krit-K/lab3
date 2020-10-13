/**
 * CS2106 AY 20/21 Semester 1 - Lab 3
 *
 * Your implementation should go in this file.
 */
#include "stdlib.h"
#include "fizzbuzz_workers.h"
#include "barrier.h" // you may use barriers if you think it can help your
                     // implementation

// declare variables to be used here

barrier_t *barrier;

// typedef struct  {
//     int n;
//     sem_t number;
//     sem_t fizz;
//     sem_t buzz;
//     sem_t fizz_buzz;
// } Fizz_Buzz;

void fizzbuzz_init ( int n ) {
    // Fizz_Buzz* obj = (Fizz_Buzz*) malloc(sizeof(Fizz_Buzz));
    barrier = malloc(sizeof(barrier_t));
    barrier_init(barrier, 4);
    // obj->n = n;
    // sem_init(&obj->number, 0, 1);
    // sem_init(&obj->fizz, 0, 0);
    // sem_init(&obj->buzz, 0, 0);
    // sem_init(&obj->fizz_buzz, 0, 0);
}

void num_thread( int n, void (*print_num)(int) ) {
    for (int i = 1; i <= n; i++) {
        //if number is not divisible by 3 and 5
        if (i % 3 && i % 5) {
            sem_wait(barrier);
            print_num(i);
            //check what should be printed next
            // if((i + 1) % 3 == 0 && (i + 1) % 5 == 0)
            //     sem_post(&obj->fizz_buzz);
            // else if ((i + 1) % 3 == 0)
            //     sem_post(&obj->fizz);
            // else if ((i + 1) % 5 == 0)
            //     sem_post(&obj->buzz);
            // else
            //     sem_post(&obj->number);
        }
    }
}

void fizz_thread( int n, void (*print_fizz)(void) ) {
    for(int i = 3; i < n; i+=3) {
        if (i % 15) {
            barrier_wait(barrier);
            print_fizz();
            // if ((i + 1) % 5 == 0) {
            //     sem_post(&obj->buzz);
            // } else {
            //     sem_post(&obj->number);
            // }
        }
    }
}

void buzz_thread( int n, void (*print_buzz)(void) ) {
    for (int i = 5; i <= n; i += 5) {
        if (i % 15) {
            sem_wait(barrier);
            print_buzz();

            // if ((i + 1) % 3 == 0)
            //     sem_post(&obj->fizz);
            // else
            //     sem_post(&obj->number);
        }
    }
}

void fizzbuzz_thread( int n, void (*print_fizzbuzz)(void) ) {
    for (int i = 15; i <= n; i += 15) {
        sem_wait(barrier);
        print_fizzbuzz();
        // sem_post(&obj->number);
    }
}

void fizzbuzz_destroy() {
     sem_destroy(barrier);
    // sem_destroy(&obj->number);
    // sem_destroy(&obj->fizz);
    // sem_destroy(&obj->buzz);
    // sem_destroy(&obj->fizz_buzz);
}
