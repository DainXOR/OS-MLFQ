#include "scheduler/scheduler.h"
#include "mlfq/mlfq.h"
#include "mlfq/pcb.h"
#include "utils/return_options.h"

#include <stdint.h>
#include <stdio.h>

#define PRIORITY_BOOST_CYCLES 20

void scheduler_priorityBoost(void* params[]){
	mlf_queue* mlfq = (mlf_queue*)params[0];

	for (int i = 1; i < LEVELS; i++) {
        while (!mlfq_isLevelEmpty(mlfq, i)) {
            PCB *p = mlfq_dequeueAt(mlfq, i);
            p->priority = 0;
            mlfq_enqueueAt(mlfq, p, 0);
        }
    }
}

void scheduler_loop(void* params[]) {
	printf("Starting scheduler...\n");
	mlf_queue* mlfq = params[0];
	mlfq_init(mlfq);
	uint64_t priorityBoostCicles = 0;

    while (1) {
        PCB *p = NULL;
        int level = -1;

        if (priorityBoostCicles >= PRIORITY_BOOST_CYCLES){
        	scheduler_priorityBoost((void *)mlfq);
        	printf("Priority boost\n");
         	priorityBoostCicles = 0;
        }

        for (int i = 0; i < LEVELS; i++) {
        	printf("Is %d queue empty? %d\n", i, mlfq_isLevelEmpty(mlfq, i));
         	printf("Front: %d | Back: %d\n", mlfq->front[i], mlfq->back[i]);
            if (mlfq_isLevelEmpty(mlfq, i) == RETURN_FALSE) {
                p = mlfq_dequeueAt(mlfq, i);
                level = i;
                printf("Executing process: %d\n", p->pid);
                break;
            }
        }

        if (!p) break;

        p->state = RUNNING;
        int slice = quantum(level);

        for (int i = 0; i < slice; i++) {

        	printf("Executing time slice: %d\n", i);
            if (pcb_executeInstruction(p))
                break;

            if (p->state == TERMINATED)
                break;
        }

        if (p->state != TERMINATED) {
            int next = level < LEVELS-1 ? level + 1 : level;

            printf("Reducing process %d priority from %d to %d\n", p->pid, p->priority, next);
            p->priority = next;

            mlfq_enqueueAt(mlfq, p, next);
            p->state = READY;
        }

        ++priorityBoostCicles;
    }

    printf("Destroying scheduler...\n");
    mlfq_destroy(mlfq);
}
