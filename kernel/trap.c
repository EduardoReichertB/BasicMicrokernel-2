#include "trap.h"
#include "timer.h"
#include "scheduler.h"
#include "uart.h"

void trap_handler(uint64_t *frame){
  uint64_t scause;
  asm volatile("csrr %0, scause" : "=r" (scause));

  uint64_t interrupcao = scause & (1ULL << 63);

  uint64_t irq = scause & ~(1UL << 63);

  //bit 63 do scause indica se acontece intererupção
  if(interrupcao && irq == 5){
    timer_next();
    schedule_from_trap(frame);
    return;
  }

  uart_print("Unhandled trap\n");
  while (1);
}