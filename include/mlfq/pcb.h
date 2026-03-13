#ifndef PCB_H
#define PCB_H

#define STACK_SIZE 1024
#define MAX_VARS   64

typedef enum {
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
} pcb_PROCESS_STATE;

typedef struct {
    int pid;

    pcb_PROCESS_STATE state;

    int arrival_time;
    int burst_time;
    int remaining_time;
    int start_time;
    int finish_time;
    int first_response_time;

    int priority;          // MLFQ level | current queue
    int pc;                // program counter

    int stack[STACK_SIZE];
    int sp;

    int vars[MAX_VARS];    // simple variable storage

    int program_size;
    int *program;          // bytecode / instruction array

} PCB;

PCB* pcb_create(int pid, int arrival_time, int burst_time, int *program, int program_size);
PCB* pcb_createRandom(void);
int pcb_executeInstruction(PCB* pcb);

#endif
