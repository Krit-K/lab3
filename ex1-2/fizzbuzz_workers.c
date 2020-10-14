/**
 * CS2106 AY 20/21 Semester 1 - Lab 3
 *
 * Your implementation should go in this file.
 */
#include "stdlib.h"
#include "fizzbuzz_workers.h"
#include "barrier.h" // you may use barriers if you think it can help your

barrier_t *barrier;

void fizzbuzz_init(int n)
{
    barrier = malloc(sizeof(barrier_t));
    barrier_init(barrier, 4);
}

void num_thread(int n, void (*print_num)(int))
{
    for (int i = 1; i <= n; i++)
    {
        //if number is not divisible by 3 and 5
        if (!(i % 3 == 0 || i % 5 == 0))
        {
            print_num(i);
        }
        barrier_wait(barrier);
    }
}

void fizz_thread(int n, void (*print_fizz)(void))
{
    for (int i = 1; i <= n; i++)
    {
        if (i % 3 == 0)
        {
            if (i % 15)
            {
                print_fizz();
            }
        }
        barrier_wait(barrier);
    }
}

void buzz_thread(int n, void (*print_buzz)(void))
{
    for (int i = 1; i <= n; i++)
    {
        if (i % 5 == 0)
        {
            if (i % 15)
            {
                print_buzz();
            }
        }
        barrier_wait(barrier);
    }
}

void fizzbuzz_thread(int n, void (*print_fizzbuzz)(void))
{
    for (int i = 1; i <= n; i++)
    {
        if (i % 15 == 0)
        {
            print_fizzbuzz();
        }
        barrier_wait(barrier);
    }
}

void fizzbuzz_destroy()
{
    sem_destroy(barrier);
}
