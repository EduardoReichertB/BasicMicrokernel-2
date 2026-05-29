#include "scheduler.h"
#include "task.h"
#include "uart.h"

extern TCB tasks[];
extern int task_count;

extern void context_switch(void*, void*);

static int current = 0;

/*   Round-Robin padrão   */

static int round_robin()
{
    return (current + 1) % task_count;
}

/*   Algoritmo atual   */

static sched_algo_t current_algo = round_robin;

void scheduler_set_algorithm(sched_algo_t algo)
{
    if (algo)
        current_algo = algo;
}

/*   Yield   */

void yield()
{
    if(task_count < 2){
        return;
    }

    int prev = current;
    int next = current_algo();

    if(prev == next){
        return;
    }

    current = next;

    context_switch(tasks[prev].regs, tasks[next].regs);
}

/*   Início   */

void scheduler_start()
{
    if (task_count == 0){
        return;
    }

    current = 0;
    tasks[0].entry();
}

void schedule_from_trap(uint64_t *frame){
    if(task_count < 2){
        return;
    }

    int prev = current;
    int next = current_algo();

    if(prev == next){
        uart_print("\nprev = next\n");
        return;
    }

    for(int i = 0; i < 31; i++){
        tasks[prev].regs[i] = frame[i];
    }

    asm volatile(
        "csrr %0, sepc" : "=r"(tasks[prev].sepc)
    );

    current = next;

    for(int i = 0; i < 31; i++){
        frame[i] = tasks[next].regs[i];
    }

    asm volatile(
        "csrw sepc, %0" :: "r"(tasks[next].sepc)
    );
}

//pegar a tarefa atual.
int scheduler_current_task(void){
    return current;
}

//testar o round_robin
int teste_rr(void){
    return (current + 1) % task_count;
}