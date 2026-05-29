#include "task.h"
#include "scheduler.h"
#include "memory.h"
#include "timer.h"
#include "trap.h"

extern void trap_entry(void); //assembly

/*   Tasks   */

void task1()
{
    int numero = 0;
    while (1)
    {
        uart_print("Task 1 running\n");
        uart_print_uint(numero++);

        //yield();
    }
}

void task2()
{
    int numero = 0;
    while (1)
    {
        uart_print("Task 2 running\n");
        uart_print_uint(numero++);

        //yield();
    }
}

void task3()
{
    int numero = 0;
    while (1)
    {
        uart_print("Task 3 running\n");
        uart_print_uint(numero++);

        //yield();
    }
}

void task4()
{
    int numero = 0;
    while (1)
    {
        uart_print("Task 4 running\n");
        uart_print_uint(numero++);

        //yield();
    }
}

/*   Kernel   */

void kernel_main()
{
    memory_init();   // OBRIGATÓRIO

    uart_print("============= Basic Kernel =============\n");

    xTaskCreate(task1, 2048, 1);
    xTaskCreate(task2, 2048, 1);
    xTaskCreate(task3, 2048, 1);
    xTaskCreate(task4, 2048, 1);

    asm volatile (
        "csrw stvec, %0" :: "r"(trap_entry)
    );

    timer_init(100000);

    scheduler_start();

    while (1);
}