#include "mlfq/pcb.h"
#include "utils/return_options.h"

#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

PCB* pcb_create(uint64_t pid, int64_t arrival_time, int64_t burst_time, int64_t *program, int64_t program_size)
{
    PCB* pcb = (PCB*)malloc(sizeof(PCB));
    if (!pcb)
        return NULL;

    pcb->pid = pid;
    pcb->state = PCB_STATE_READY;

    pcb->arrivalTime = arrival_time;
    pcb->burstTime = burst_time;
    pcb->remainingTime = burst_time;

    pcb->startTime = -1;
    pcb->finishTime = -1;
    pcb->firstResponseTime = -1;

    pcb->priority = 0;   // start at highest queue
    pcb->pc = 0;

    pcb->sp = 0;

    memset(pcb->stack, 0, sizeof(pcb->stack));
    memset(pcb->vars, 0, sizeof(pcb->vars));

    pcb->programSize = program_size;
    pcb->program = program;

    return pcb;
}


PCB* pcb_createRandom(void)
{
	static uint64_t pid = 1;
    int64_t arrivalTime = rand() % 10;  // arrivals between 0–9
    int64_t burstTime = (rand() % (PCB_CONFIG_MAX_BURST - PCB_CONFIG_MIN_BURST + 1)) + PCB_CONFIG_MIN_BURST;

    int64_t programSize = (rand() % PCB_CONFIG_MAX_PROGRAM_SIZE) + 1;

    int64_t *program = malloc(sizeof(int64_t) * (unsigned long)programSize);
    if (!program)
        return NULL;

    for (int i = 0; i < programSize; i++) {
        program[i] = rand(); // dummy instruction
    }

    PCB* pcb = pcb_create(pid, arrivalTime, burstTime, program, programSize);
    ++pid;
    return pcb;
}

PCB* pcb_createRandomAt(int64_t arrivalTime){
	PCB* tmp = pcb_createRandom();
	tmp->arrivalTime = arrivalTime;

	return tmp;
}

return_code pcb_executeInstruction(PCB* pcb){
	++(pcb->pc);
	int64_t instruction = *(pcb->program + pcb->pc);
	(void) instruction;

	return RETURN_NORMAL;
}
