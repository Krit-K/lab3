/**
 * CS2106 AY 20/21 Semester 1 - Lab 3
 *
 * This file contains declarations. You should only modify the fifo_sem_t struct,
 * as the method signatures will be needed to compile with the runner.
 */
#include "exit_controller.h"
#include "stdlib.h"
#include "stdio.h"

void exit_controller_init(exit_controller_t *exit_controller, int no_of_priorities)
{
    exit_controller->noOfPriorities = no_of_priorities;
    exit_controller->highestPriority = no_of_priorities - 1;
    exit_controller->isFirst = 1;
    exit_controller->hasQueue = 0;

    exit_controller->priorityArrPtr = malloc(sizeof(int) * no_of_priorities);
    for (int i = 0; i < no_of_priorities; i++)
    {
        *(exit_controller->priorityArrPtr + i) = 0;
    }

    sem_init(&(exit_controller->mutex1), 0, 1);
    sem_init(&(exit_controller->semaphore), 0, 1);

    exit_controller->semaphoreArrPtr = malloc(sizeof(sem_t) * no_of_priorities);
    //exit_controller->semaphoreArrPtr = malloc(sizeof(sem_t) * no_of_priorities);
    for (int i = 0; i < no_of_priorities; i++)
    {
        sem_init(exit_controller->semaphoreArrPtr + i, 0, 0);
    }
}

void exit_controller_wait(exit_controller_t *exit_controller, int priority)
{
    sem_wait(&(exit_controller->mutex1));
    exit_controller->isFirst = 1;
    for (int i = 0; i < exit_controller->noOfPriorities; i++)
    {
        if (*(exit_controller->priorityArrPtr + i) != 0)
        {
            exit_controller->isFirst = 0;
        }
    }

    (*(exit_controller->priorityArrPtr + priority))++;

    if (exit_controller->isFirst != 1)
    {
        sem_post(&(exit_controller->mutex1));
        sem_wait((exit_controller->semaphoreArrPtr + priority));
    }
    else
    {
        sem_post(&(exit_controller->mutex1));
    }

    sem_wait(&(exit_controller->semaphore));
}

void exit_controller_post(exit_controller_t *exit_controller, int priority)
{
    sem_wait(&(exit_controller->mutex1));
    exit_controller->hasQueue = 0;

    exit_controller->highestPriority = exit_controller->noOfPriorities - 1;

    (*(exit_controller->priorityArrPtr + priority))--;
    for (int i = 0; i < exit_controller->noOfPriorities; i++)
    {
        if (*(exit_controller->priorityArrPtr + i) != 0)
        {
            exit_controller->hasQueue = 1;
            exit_controller->highestPriority = i;
            break;
        }
    }
    if (exit_controller->hasQueue == 1)
    {
        // We call for (one of) the thread that has the highest priority
        sem_post((exit_controller->semaphoreArrPtr + exit_controller->highestPriority));
    }
    sem_post(&(exit_controller->semaphore));
    sem_post(&(exit_controller->mutex1));
}

void exit_controller_destroy(exit_controller_t *exit_controller)
{
    for (int i = 0; i < exit_controller->noOfPriorities; i++)
    {
        sem_destroy(exit_controller->semaphoreArrPtr + i);
    }
    sem_destroy(&(exit_controller->mutex1));
    sem_destroy(&(exit_controller->semaphore));
    free(exit_controller->priorityArrPtr);
    free(exit_controller->semaphoreArrPtr);
}
