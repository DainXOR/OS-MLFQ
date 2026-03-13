#include "mlfq/mlfq.h"
#include "utils/return_options.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int quantum(int level){
	static int quantum[LEVELS] = {2, 4, 8};
	return quantum[level];
}

int8_t mlfq_init(mlf_queue* queue){
	for (int i = 0; i < LEVELS; i++) {
		for (int j = 0; j < MAX_PROCESSES; j++) {
			queue->queue[i][j] = NULL;
		}

		queue->front[i] = 0;
        queue->rear[i] = 0;
	}

	return 0;
}
int8_t mlfq_destroy(mlf_queue* queue){
	(void) queue;
	return 0;
}

int8_t mlfq_enqueueAt(mlf_queue* queue, PCB* pcb, int level)
{
    if (level < 0 || level >= LEVELS)
        return -1;

    if (queue->rear[level] >= MAX_PROCESSES)
        return -2;

    printf("Enqueue process %d\n", pcb->pid);
    queue->queue[level][queue->rear[level]] = pcb;
    queue->rear[level]++;

    return 0;
}
PCB* mlfq_dequeueAt(mlf_queue* queue, int level)
{
    if (level < 0 || level >= LEVELS)
        return NULL;

    if (queue->front[level] == queue->rear[level])
        return NULL;

    PCB* pcb = queue->queue[level][queue->front[level]];
    queue->front[level]++;
    printf("Dequeue process %d\n", pcb->pid);

    return pcb;
}

return_code mlfq_isLevelEmpty(mlf_queue* queue, int level)
{
    if (level < 0 || level >= LEVELS)
        return RETURN_ERROR;

    for (int j = 0; j < MAX_PROCESSES; j++) {
		PCB* pcb = queue->queue[level][j];

		if (pcb) return RETURN_FALSE;
	}

    return RETURN_TRUE;
}

PCB* mlfq_getNext(mlf_queue* q)
{
    for (int i = 0; i < LEVELS; i++) {
        if (!mlfq_isLevelEmpty(q, i))
            return mlfq_dequeueAt(q, i);
    }

    return NULL;
}
