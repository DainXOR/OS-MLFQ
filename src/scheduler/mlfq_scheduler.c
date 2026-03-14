#include "scheduler/scheduler.h"
#include "mlfq/mlfq.h"
#include "mlfq/pcb.h"
#include "utils/return_options.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define PRIORITY_BOOST_CYCLES 20

void scheduler_priorityBoost(mlf_queue* mlfq){
	for (int i = 1; i < LEVELS; i++) {
        while (mlfq_isLevelEmpty(mlfq, i) == RETURN_FALSE) {
            PCB *p = mlfq_dequeueAt(mlfq, i);
            p->priority = 0;
            mlfq_enqueueAt(mlfq, p, 0);
        }
    }
}

void scheduler_loop(void* params[]) {
	printf("Starting scheduler...\n");
	mlf_queue* mlfq = (mlf_queue*)params[0];
	uint64_t totalCycles = 0;
	uint64_t priorityBoostCycles = 0;

    while (1) {
        PCB *p = NULL;
        int8_t level = -1;

        if (priorityBoostCycles >= PRIORITY_BOOST_CYCLES){
        	printf("Priority boost\n");
        	scheduler_priorityBoost(mlfq);
         	printf("Priority boost finish\n");
         	priorityBoostCycles = 0;
        }

        for (int8_t i = 0; i < LEVELS; i++) {
        	printf("Is %d queue empty? %d\n", i, mlfq_isLevelEmpty(mlfq, i));
         	printf("Front: %d | Back: %d\n", mlfq->front[i], mlfq->back[i]);
            if (mlfq_isLevelEmpty(mlfq, i) == RETURN_FALSE) {
                p = mlfq_dequeueAt(mlfq, i);

                if (p->state != PCB_STATE_READY){
                	printf("Process %lu is not ready, skipping", p->pid);
               		mlfq_enqueueAt(mlfq, p, i);
                	continue;
                }
                else {
	                printf("Executing process: %lu\n", p->pid);
	               	level = i;
	                break;
                }
            }
        }

        if (!p) break;

        p->state = PCB_STATE_RUNNING;
        int slices = quantum(level);
        printf("Time slice size: %d\n", slices);

        for (int i = 0; i < slices; i++) {
        	printf("Executing time slice: %d\n", i);

        	if (p->startTime == -1) {
				p->startTime = (int)totalCycles;
				p->firstResponseTime = p->startTime - p->arrivalTime;
			}

            if (pcb_executeInstruction(p) == RETURN_ERROR){
           		printf("Process %lu blocked", p->pid);
            	p->state = PCB_STATE_BLOCKED;
            	break;
            }

            --(p->remainingTime);
            if (!p->remainingTime){
           		printf("Finished process %lu\n", p->pid);
                p->state = PCB_STATE_TERMINATED;
            }

            if (p->state == PCB_STATE_TERMINATED)
                break;
        }

        if (p->state != PCB_STATE_TERMINATED) {
            int8_t next = level < LEVELS-1 ? level + 1 : level;

            printf("Reducing process %lu priority from %d to %d\n", p->pid, p->priority, next);
            p->priority = next;

            mlfq_enqueueAt(mlfq, p, next);
            p->state = PCB_STATE_READY;
        } else {
        	printf("Deleting process %lu since it is terminated\n", p->pid);
       		free(p);
        }

        ++totalCycles;
        ++priorityBoostCycles;
        printf("%lu cycle\n", totalCycles);
    }

    printf("All processes terminated, destroying scheduler...\n");
    mlfq_destroy(mlfq);
}
