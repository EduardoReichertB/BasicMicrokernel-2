#include "timer.h"
#include "riscv.h"

static uint64_t tick_interval = 100000;

void timer_next(void)
{
  uint64_t now;
  uint64_t next;

  asm volatile ("rdtime %0" : "=r"(now)); //lê o registrador mtime
  next = now + tick_interval;

  sbi_set_timer(next); //quando o mtime for maior do que o mtimecmp então temos um sinal de interrupção

  //esse mtimecmp é um registrador da machine mode e acho que o openSBI ja lida com ele sem precisarmos mexer em nada
}

void timer_init(uint64_t interval){
  if (interval != 0){
    tick_interval = interval;
    timer_next();
  }

  timer_next();

  /* Habilita STIE (bit 5 do sie)*/
  habilitar_STIE();

  /* Habilita SIE global no sstatus (bit 1)*/
  habilitar_bit1_sstatus();
}