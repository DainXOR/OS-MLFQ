#include "scheduler/scheduler.h"
#include "mlfq/mlfq.h"
#include "mlfq/pcb.h"
#include "utils/return_options.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum {
	PRIORITY_BOOST_CYCLES = 20
};

#include <stdio.h>

#include <sys/stat.h>

void scheduler_prepareAssets(void){
    mkdir("assets", 0777);
}
void scheduler_initResultsFile(void) {
	scheduler_prepareAssets();
    FILE *f = fopen("assets/results.csv", "r");

    /* If file exists, just close and return */
    if (f) {
        fclose(f);
        return;
    }

    /* File does not exist → create it and write header */
    f = fopen("assets/results.csv", "w");
    if (!f) {
        perror("Could not create assets/results.csv");
        return;
    }

    fprintf(f, "PID,Arrival,Burst,Start,Finish,Response,Turnaround,Waiting\n");
    fclose(f);
}
void scheduler_writeResult(PCB* p){
    FILE *f = fopen("assets/results.csv", "a");
    if (!f) {
        perror("Could not open results.csv");
        return;
    }
    uint64_t pid = p->pid;
    int64_t arrival = p->arrivalTime;
    int64_t burst = p->burstTime;
    int64_t start = p->startTime;
    int64_t finish = p->finishTime;
    int64_t response = p->firstResponseTime;

    int64_t turnaround = finish - arrival;
    int64_t waiting = turnaround - burst;

    fprintf(
        f,
        "%lu,%ld,%ld,%ld,%ld,%ld,%ld,%ld\n",
        pid,
        arrival,
        burst,
        start,
        finish,
        response,
        turnaround,
        waiting
    );

    fclose(f);
}

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
	scheduler_initResultsFile();
	mlf_queue* mlfq = (mlf_queue*)params[0];
	uint64_t totalCycles = 0;
	uint64_t priorityBoostCycles = 0;
	return_code mustWait = RETURN_FALSE;
	printf("Scheduler begin run\n");

    while (1) {
        PCB *p = NULL;
        int8_t level = -1;
        mustWait = RETURN_FALSE;

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
                	printf("Process %lu is not ready, skipping\n", p->pid);
               		mlfq_enqueueAt(mlfq, p, i);
                 	p = NULL;
                	continue;
                }
                else if ((uint64_t)p->arrivalTime > totalCycles){
                	printf("Process %lu has not yet arrived, skipping\n", p->pid);
                 	printf("Cycles to arrive: %lu\n", (uint64_t)p->arrivalTime - totalCycles);
                 	mlfq_enqueueAt(mlfq, p, i);
                  	mustWait = RETURN_TRUE;
                   	p = NULL;
                  	continue;
                }
                else {
	                printf("Executing process: %lu\n", p->pid);
	               	level = i;
	                break;
                }
            }
        }

        if (!p && mustWait == RETURN_FALSE) break;
        else if (!p && mustWait == RETURN_TRUE) { // Wait for the other processes to arrive, no priority boost since is noop
        	totalCycles++;
         	printf("%lu cycle\n", totalCycles);
         	continue;
        }

        p->state = PCB_STATE_RUNNING;
        int slices = quantum(level);
        int usedSlices = 0;
        printf("Time slice size: %d\n", slices);

        for (int i = 0; i < slices; i++) {
        	totalCycles++;
	        priorityBoostCycles++;
	        printf("%lu cycle\n", totalCycles);
        	printf("Executing time slice: %d\n", i);


        	if (p->startTime == -1) {
				p->startTime = (int)totalCycles;
				p->firstResponseTime = p->startTime - p->arrivalTime;
			}

            if (pcb_executeInstruction(p) == RETURN_ERROR){
           		printf("Process %lu blocked\n", p->pid);
            	p->state = PCB_STATE_BLOCKED;
            	usedSlices++;
            	break;
            }

            --(p->remainingTime);
            if (!p->remainingTime){
           		printf("Finished process %lu\n", p->pid);
                p->state = PCB_STATE_TERMINATED;
            }

            if (p->state == PCB_STATE_TERMINATED){
            	p->finishTime = (int64_t)totalCycles;
           		usedSlices++;
                break;
            }
        }

        if (p->state != PCB_STATE_TERMINATED) {
            int8_t next = level < LEVELS-1 && usedSlices == slices ? level + 1 : level;

            printf("Moving process %lu priority from %d to %d\n", p->pid, p->priority, next);
            p->priority = next;

            mlfq_enqueueAt(mlfq, p, next);
            p->state = PCB_STATE_READY;
        } else {
        	printf("Deleting process %lu since it is terminated\n", p->pid);
         	printf("Generating metrics\n");
         	scheduler_writeResult(p);
          	printf("Metrics stored for process %lu\n", p->pid);
       		free(p);
        	printf("Process deleted\n");
        }
    }

    printf("All processes terminated, destroying scheduler...\n");
    mlfq_destroy(mlfq);
}
