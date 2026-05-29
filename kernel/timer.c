#include "timer.h"
#include "riscv.h"

static uint64_t tick_interval = 100000;

static inline void sbi_set_timer(uint64_t time) {
  asm volatile(
    "mv a0, %0\n"
    "li a7, 0\n"
    "ecall" :: "r"(time) : "a0", "a7", "memory"
  );
}

//TESTE
//LÊ O VALOR DE CST TIME
static inline uint64_t read_time() {
  uint64_t time;
  __asm__ volatile ("rdtime %0" : "=r"(time));
  return time;
}

uint64_t ler_tempo(){
  return read_time();
}

void timer_next(void)
{
  uint64_t now;
  uint64_t next;

  asm volatile ("rdtime %0" : "=r"(now));
  next = now + tick_interval;

  sbi_set_timer(next);
}

void timer_init(uint64_t interval){
  if (interval != 0){
    tick_interval = interval;
    timer_next();
  }

  timer_next();

  /* Habilita STIE (bit 5 do sie) usando csrs com registrador */
  habilitar_STIE();

  /* Habilita SIE global no sstatus (bit 1) – imediato 2 cabe nos 5 bits */
  habilitar_bit1_sstatus();
}