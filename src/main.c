#include <stdio.h>
#include <stdlib.h>

#include "mlfq/mlfq.h"
#include "mlfq/pcb.h"
#include "scheduler/scheduler.h"

int main(void){
	mlf_queue mlfq;
	mlfq_init(&mlfq);

	mlfq_enqueueAt(&mlfq, pcb_createRandomAt(0), 0);
	mlfq_enqueueAt(&mlfq, pcb_createRandomAt(3), 0);
	mlfq_enqueueAt(&mlfq, pcb_createRandomAt(20), 0);
	mlfq_enqueueAt(&mlfq, pcb_createRandomAt(26), 0);

	void* args[1] = { &mlfq };
	scheduler_loop(args);

	return 0;
}
