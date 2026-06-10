#ifndef RISCV_H
#define RISCV_H

#include <stdint.h>

//Supervisor Interupt Enable (SIE) - habilita algumas interrupções
//bit 5 = STIE
static inline void habilitar_STIE() {  //habilitar_bit5_sie
    asm volatile("csrs sie, %0" :: "r"(1ULL << 5));
}

//Supervisor Status
//bit 1 = habilita o registrador SIE
static inline void habilitar_bit1_sstatus(){
  asm volatile ("csrs sstatus, %0" :: "r"(1ULL << 1));
}

//----

/* o registrador a7 é usado apra informar qual a função SBI será usado
  o ecall passa para o modo de maquina e le os dois registradores, a0(argmento) e a7(indica a função)

  pelo que entendi, o "modo maquina" é feito pelo openSBI do QEMU e já está pronto*/
static inline void sbi_set_timer(uint64_t time) {
  asm volatile(
    "mv a0, %0\n" //copia o valor de time para o registrador a0
    "li a7, 0\n"  //"load integer", grava o valor 0 no registrador a7
    "ecall" :: "r"(time) : "a0", "a7", "memory"
  );
}

//---

//lê o registrador scause (supervisor cause)
/*
  o bit 63 do scause indica interrupção(1) ou exceção(0)
  os bits 0-62 indicam o codigo da causa, o código 5 é para timer supervisor (o que nós usamos)
  é possivel ver que no codigo do trap a gente ve o bit 63 pra ver se teve interrupção e vemos se os
  outros bits formam o codigo 5, se os dois forem verdadeiros então tem interrupção
*/
static inline uint64_t ler_scause(){
  uint64_t x;
  asm volatile("csrr %0, scause" : "=r"(x));
  return x;
}

//----

// spec = SUPERVISOR PROGRAM EXCEPTION COUNTER
// guarda o endereço da instrução onde a trap ocorreu, quando voltar, volta do mesmo ponto
/*
  quando uma interrupção acontece o PC é salvo em sepc
  e desvia a execução para o tratador de traps guardado em stvec
*/
static inline uint64_t ler_sepc(){
  uint64_t x;
  asm volatile("csrr %0, sepc" : "=r"(x));
  return x;
}

static inline void escrever_sepc(uint64_t x){
  asm volatile("csrw sepc, %0" :: "r"(x));
}

//----

static inline void escrever_stvec(void* x){
  asm volatile("csrw stvec, %0" :: "r"(x));
}

#endif