#ifndef PCB_H
#define PCB_H

#include "utils/return_options.h"
#include <stdint.h>
typedef enum {
	PCB_CONFIG_STACK_SIZE = 1024,
	PCB_CONFIG_MAX_VARS = 64,
	PCB_CONFIG_MIN_BURST = 2,
	PCB_CONFIG_MAX_BURST = 60,
	PCB_CONFIG_MAX_PROGRAM_SIZE = 32,
} pcb_configs;

typedef enum {
    PCB_STATE_READY,
    PCB_STATE_RUNNING,
    PCB_STATE_BLOCKED,
    PCB_STATE_TERMINATED
} pcb_process_state;

typedef struct {
    uint64_t pid;

    int64_t arrivalTime;
    int64_t burstTime;
    int64_t remainingTime;
    int64_t startTime;
    int64_t finishTime;
    int64_t firstResponseTime;

    pcb_process_state state;
    int8_t priority;          // MLFQ level | current queue
    int64_t pc;                // program counter

    int64_t stack[PCB_CONFIG_STACK_SIZE];
    int64_t sp;

    int64_t vars[PCB_CONFIG_MAX_VARS];    // simple variable storage

    int64_t programSize;
    int64_t *program;          // bytecode / instruction array

} PCB;

PCB* pcb_create(uint64_t pid, int64_t arrival_time, int64_t burst_time, int64_t *program, int64_t program_size);
PCB* pcb_createRandom(void);
return_code pcb_executeInstruction(PCB* pcb);

#endif
