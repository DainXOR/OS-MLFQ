#include <stdio.h>
#include <stdlib.h>

#include "mlfq/mlfq.h"
#include "mlfq/pcb.h"
#include "scheduler/scheduler.h"

int main(void){
	mlf_queue mlfq;
	mlfq_init(&mlfq);

	printf("Enqueue result: %d\n", mlfq_enqueueAt(&mlfq, pcb_createRandom(), 0));
	printf("Enqueue result: %d\n", mlfq_enqueueAt(&mlfq, pcb_createRandom(), 0));
	printf("Enqueue result: %d\n", mlfq_enqueueAt(&mlfq, pcb_createRandom(), 0));
	printf("Enqueue result: %d\n", mlfq_enqueueAt(&mlfq, pcb_createRandom(), 0));

	scheduler_loop((void*)&mlfq);

	return 0;
}
