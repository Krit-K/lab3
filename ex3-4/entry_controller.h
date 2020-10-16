#ifndef __CS2106_ENTRY_CONTROLLER_H_
#define __CS2106_ENTRY_CONTROLLER_H_

#include <semaphore.h>

#define ENTRY_CONTROLLER_MAX_USES 5000 // we impose a limit on the number of uses we can \
                                       // have

typedef struct entry_controller
{
    sem_t logicGate;
    sem_t gate2;
    sem_t gate3;
    int queueStart;
    int queueEnd;
    int loadingBays;
    sem_t semaphore;
    sem_t *queueHead;
} entry_controller_t;

void entry_controller_init(entry_controller_t *entry_controller, int loading_bays);
void entry_controller_wait(entry_controller_t *entry_controller);
void entry_controller_post(entry_controller_t *entry_controller);
void entry_controller_destroy(entry_controller_t *entry_controller);

#endif // __CS2106_ENTRY_CONTROLLER_H_
