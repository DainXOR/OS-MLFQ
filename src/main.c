#include <stdio.h>
#include <stdlib.h>

#include "mlfq/mlfq.h"
#include "mlfq/pcb.h"
#include "scheduler/scheduler.h"

int main(void){
	mlf_queue mlfq;
	mlfq_init(&mlfq);

	PCB* tmp = pcb_createRandomAt(0);
	tmp->burstTime = 8;
	mlfq_enqueueAt(&mlfq, tmp, 0);

	tmp = pcb_createRandomAt(1);
	tmp->burstTime = 4;
	mlfq_enqueueAt(&mlfq, tmp, 0);

	tmp = pcb_createRandomAt(2);
	tmp->burstTime = 9;
	mlfq_enqueueAt(&mlfq, tmp, 0);

	tmp = pcb_createRandomAt(3);
	tmp->burstTime = 5;
	mlfq_enqueueAt(&mlfq, tmp, 0);

	void* args[1] = { &mlfq };
	scheduler_loop(args);

	return 0;
}
