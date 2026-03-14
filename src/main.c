#include <stdio.h>
#include <stdlib.h>

#include "mlfq/mlfq.h"
#include "mlfq/pcb.h"
#include "scheduler/scheduler.h"

int main(void){
	mlf_queue mlfq;
	mlfq_init(&mlfq);

	PCB* tmp = pcb_createRandomAt(0);
	if (!tmp) goto error_exit;
	tmp->burstTime = 8;
	mlfq_enqueueAt(&mlfq, tmp, 0);

	tmp = pcb_createRandomAt(1);
	if (!tmp) goto error_exit;
	tmp->burstTime = 4;
	mlfq_enqueueAt(&mlfq, tmp, 0);

	tmp = pcb_createRandomAt(2);
	if (!tmp) goto error_exit;
	tmp->burstTime = 9;
	mlfq_enqueueAt(&mlfq, tmp, 0);

	tmp = pcb_createRandomAt(3);
	if (!tmp) goto error_exit;
	tmp->burstTime = 5;
	mlfq_enqueueAt(&mlfq, tmp, 0);

	void* args[1] = { &mlfq };
	scheduler_loop(args);

	return 0;

	error_exit:
	printf("Error creating pcb, terminating program.");
	mlfq_destroyContents(&mlfq);
	return -1;
}
