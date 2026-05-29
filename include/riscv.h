#ifndef RISCV_H
#define RISCV_H

#include <stdint.h>

//Supervisor Interupt Enable (SIE) - habilita algumas interrupções
//bit 5 = STIE
static inline void habilitar_STIE() {
    asm volatile("csrs sie, %0" :: "r"(1ULL << 5));
}

//Supervisor Status
//bit 1 = habilita o registrador SIE
static inline void habilitar_bit1_sstatus(){
  asm volatile ("csrs sstatus, %0" :: "r"(1ULL << 1));
}

#endif