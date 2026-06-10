#include "scheduler.h"
#include "task.h"
#include "uart.h"
#include "riscv.h"

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

    //salva os registradores da task anterior e carrega os registradores da nova task
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

//frame aponta para uma estrutura que contem todos os registradores salvos no momento em que ocorreu a interrupção
//antes de chamar trap_handler
void schedule_from_trap(uint64_t *frame){
    if(task_count < 2){
        return;
    }

    int prev = current;
    int next = current_algo(); //seleciona proxima tarefa com round robin

    if(prev == next){
        uart_print("\nprev = next\n");
        return;
    }

    //salva o estado da tarefa atual
    //como o frame contem os registradores antes da interrupção, nós salvamos todos para podermos carregar os da nova tarefa
    for(int i = 0; i < 31; i++){
        tasks[prev].regs[i] = frame[i];
    }
    tasks[prev].sepc = ler_sepc();

    current = next;

    //carregamos o estado da proxima tarefa
    for(int i = 0; i < 31; i++){
        frame[i] = tasks[next].regs[i];
    }
    escrever_sepc(tasks[next].sepc);
}

//pegar a tarefa atual.
int scheduler_current_task(void){
    return current;
}

//testar o round_robin
int teste_rr(void){
    return (current + 1) % task_count;
}