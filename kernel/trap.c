#include "trap.h"
#include "timer.h"
#include "scheduler.h"
#include "uart.h"
#include "riscv.h"

void trap_handler(uint64_t *frame){
  uint64_t scause = ler_scause();

  //ve o bit 63, se = 1 então é interrupção
  uint64_t interrupcao = scause & (1ULL << 63);

  //ve os bits 0-62, se = 5 é o codigo de timer supervisor, ou seja, interrupção por tempo
  uint64_t codigo = scause & ~(1UL << 63);

  //bit 63 do scause indica se acontece intererupção e se o codigo for 5
  //melhor explicado no "riscv.h" na função "ler_scause()"
  if(interrupcao && codigo == 5){
    timer_next();
    schedule_from_trap(frame);
    return;
  }

  //não deve chegar aqui
  uart_print("Unhandled trap\n");
  while (1);
}