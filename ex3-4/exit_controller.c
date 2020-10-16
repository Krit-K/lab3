#include "stdio.h"
#include "exit_controller.h"
#include "stdlib.h"

void exit_controller_init(exit_controller_t *exit_controller, int no_of_priorities)
{
    // allocate memory
    exit_controller->priorityPtr = malloc(sizeof(int) * no_of_priorities);
    exit_controller->semaphorePtr = malloc(sizeof(sem_t) * no_of_priorities);

    exit_controller->prioritiesNum = no_of_priorities;
    exit_controller->highestPriority = no_of_priorities - 1;
    exit_controller->isFirst = 1;
    exit_controller->hasQueue = 0;

    int i = 0;
    while (i < exit_controller->prioritiesNum)
    {
        *(exit_controller->priorityPtr + i) = 0;
        i += 1;
    }

    sem_init(&exit_controller->semaphore, 0, 1);
    sem_init(&exit_controller->gate, 0, 1);

    i = 0;
    while (i < exit_controller->prioritiesNum)
    {
        sem_init(exit_controller->semaphorePtr + i, 0, 0);
        i += 1;
    }
}

void exit_controller_wait(exit_controller_t *exit_controller, int priority)
{
    sem_wait(&exit_controller->gate); // 0
    exit_controller->isFirst = 1;
    for (int i = 0; i < exit_controller->prioritiesNum; i++)
    {
        if (*(exit_controller->priorityPtr + i) != 0)
        {
            exit_controller->isFirst = 0;
        }
    }

    (*(exit_controller->priorityPtr + priority))++;

    if (exit_controller->isFirst > 0)
    {
        sem_post(&exit_controller->gate);
    }
    else
    {
        sem_post(&exit_controller->gate);
        sem_wait(exit_controller->semaphorePtr + priority);
    }

    sem_wait(&exit_controller->semaphore);
}

void exit_controller_post(exit_controller_t *exit_controller, int priority)
{
    sem_wait(&exit_controller->gate);
    exit_controller->hasQueue = 0;

    exit_controller->highestPriority = exit_controller->prioritiesNum - 1;
    (*(exit_controller->priorityPtr + priority))--;
    for (int i = 0; i < exit_controller->prioritiesNum; i++)
    {
        if (*(exit_controller->priorityPtr + i) != 0)
        {
            exit_controller->hasQueue = 1;
            exit_controller->highestPriority = i;
            break;
        }
    }

    if (exit_controller->hasQueue > 0)
    {
        sem_post(exit_controller->semaphorePtr + (exit_controller->highestPriority));
    }
    sem_post(&exit_controller->semaphore);
    sem_post(&exit_controller->gate);
}

void exit_controller_destroy(exit_controller_t *exit_controller)
{
    int i = 0;
    while (i < exit_controller->prioritiesNum)
    {
        sem_destroy(exit_controller->semaphorePtr + i);
        i++;
    }
    sem_destroy(&exit_controller->gate);
    sem_destroy(&exit_controller->semaphore);
}
