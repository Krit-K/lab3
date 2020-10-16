#include "entry_controller.h"
#include "semaphore.h"
#include "stdio.h"
#include "stdlib.h"

void entry_controller_init(entry_controller_t *entry_controller, int loading_bays)
{
    sem_init(&entry_controller->logicGate, 0, 1); //binary sempahore for signalling
    sem_init(&entry_controller->gate2, 0, 1);
    sem_init(&entry_controller->gate3, 0, 1);
    sem_init(&entry_controller->semaphore, 0, loading_bays); //modifying data
    entry_controller->queueHead = malloc(sizeof(sem_t) * ENTRY_CONTROLLER_MAX_USES);
    entry_controller->queueStart = 0;
    entry_controller->queueEnd = 0;
    entry_controller->loadingBays = loading_bays;

    int i = 0;
    while (i < ENTRY_CONTROLLER_MAX_USES)
    {
        sem_init(entry_controller->queueHead + i, 0, 0);
        i++;
    }
}

void entry_controller_wait(entry_controller_t *entry_controller)
{
    sem_wait(&entry_controller->logicGate);
    sem_wait(&entry_controller->gate2);

    // check if the queue is not empty after checks whether loading bay is empty
    if (entry_controller->queueStart != entry_controller->queueEnd || entry_controller->loadingBays == 0)
    {
        int queueNumber = entry_controller->queueEnd;
        entry_controller->queueEnd = (entry_controller->queueEnd + 1) % ENTRY_CONTROLLER_MAX_USES;
        sem_post(&entry_controller->gate2);
        sem_post(&entry_controller->logicGate);
        sem_wait(entry_controller->queueHead + queueNumber);
        sem_wait(&entry_controller->gate2);
        entry_controller->queueStart = (entry_controller->queueStart + 1) % ENTRY_CONTROLLER_MAX_USES;
        sem_post(&entry_controller->gate2);
    }
    else
    {
        sem_post(&entry_controller->gate2);
        sem_post(&entry_controller->logicGate);
    }
    sem_wait(&entry_controller->semaphore);
    sem_wait(&entry_controller->gate3);
    entry_controller->loadingBays--;
    sem_post(&entry_controller->gate3);
}
void entry_controller_post(entry_controller_t *entry_controller)
{
    sem_wait(&(entry_controller->gate2));
    if (entry_controller->queueStart != entry_controller->queueEnd)
    { //there is a queue then go in if, signal to the queue
        sem_post(entry_controller->queueHead + entry_controller->queueStart);
    }
    sem_wait(&(entry_controller->gate3));
    entry_controller->loadingBays++;
    sem_post(&(entry_controller->gate3));
    sem_post(&(entry_controller->gate2));
    sem_post(&(entry_controller->semaphore));
}

void entry_controller_destroy(entry_controller_t *entry_controller)
{
    sem_destroy(&(entry_controller->logicGate));
    sem_destroy(&(entry_controller->gate2));
    sem_destroy(&(entry_controller->gate3));
    sem_destroy(&(entry_controller->semaphore));
    for (int i = 0; i < ENTRY_CONTROLLER_MAX_USES; i++)
    {
        sem_destroy(entry_controller->queueHead + i);
    }
    free(entry_controller->queueHead);
}
