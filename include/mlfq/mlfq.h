#ifndef MLFQ_H
#define MLFQ_H

#include "pcb.h"
#include "utils/return_options.h"

#include <stdint.h>

#define LEVELS 3
#define MAX_PROCESSES 128

typedef struct {
    PCB* queue[LEVELS][MAX_PROCESSES];
    int front[LEVELS];
    int back[LEVELS];
} mlf_queue;

//int quantum[LEVELS] = {2, 4, 8};

int quantum(int level);

int8_t mlfq_init(mlf_queue* queue);
int8_t mlfq_destroy(mlf_queue* queue);

int8_t mlfq_enqueueAt(mlf_queue* queue, PCB* pcb, int level);
PCB* mlfq_dequeueAt(mlf_queue* queue, int level);

return_code mlfq_isLevelEmpty(mlf_queue* queue, int level);

#endif
