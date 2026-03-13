#include "mlfq/pcb.h"

#include <stdlib.h>
#include <memory.h>

PCB* pcb_create(int pid, int arrival_time, int burst_time, int *program, int program_size)
{
    PCB* pcb = (PCB*)malloc(sizeof(PCB));
    if (!pcb)
        return NULL;

    pcb->pid = pid;
    pcb->state = READY;

    pcb->arrival_time = arrival_time;
    pcb->burst_time = burst_time;
    pcb->remaining_time = burst_time;

    pcb->start_time = -1;
    pcb->finish_time = -1;
    pcb->first_response_time = -1;

    pcb->priority = 0;   // start at highest queue
    pcb->pc = 0;

    pcb->sp = 0;

    memset(pcb->stack, 0, sizeof(pcb->stack));
    memset(pcb->vars, 0, sizeof(pcb->vars));

    pcb->program_size = program_size;
    pcb->program = program;

    return pcb;
}

#define MIN_BURST 2
#define MAX_BURST 12
#define MAX_PROGRAM_SIZE 32

PCB* pcb_createRandom(void)
{
	static int pid = 1;
    int arrival_time = rand() % 10;  // arrivals between 0–9
    int burst_time = (rand() % (MAX_BURST - MIN_BURST + 1)) + MIN_BURST;

    int program_size = (rand() % MAX_PROGRAM_SIZE) + 1;

    int *program = malloc(sizeof(int) * (unsigned long)program_size);
    if (!program)
        return NULL;

    for (int i = 0; i < program_size; i++) {
        program[i] = rand() % 10; // dummy instruction
    }

    PCB* pcb = pcb_create(pid, arrival_time, burst_time, program, program_size);
    ++pid;
    return pcb;
}

int pcb_executeInstruction(PCB* pcb){
	(void) pcb;

	return 0;
}
