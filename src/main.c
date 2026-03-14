#include <stdio.h>
#include <stdlib.h>

#include "mlfq/mlfq.h"
#include "mlfq/pcb.h"
#include "scheduler/scheduler.h"

int main(void){
	mlf_queue mlfq;
	mlfq_init(&mlfq);

	mlfq_enqueueAt(&mlfq, pcb_createRandom(), 0);
	mlfq_enqueueAt(&mlfq, pcb_createRandom(), 0);
	mlfq_enqueueAt(&mlfq, pcb_createRandom(), 0);
	mlfq_enqueueAt(&mlfq, pcb_createRandom(), 0);

	void* args[1] = { &mlfq };
	scheduler_loop(args);

	return 0;
}
