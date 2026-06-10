#include "memory.h"
#include <stdint.h>

/*   Configuração do heap   */

#define HEAP_START 0x80400000UL
#define HEAP_SIZE  (8 * 1024 * 1024)   // 8 MB

static uint8_t *heap_base = (uint8_t*)HEAP_START;
static uint8_t *heap_end  = (uint8_t*)(HEAP_START + HEAP_SIZE);

//indica o deslocamento do proximo bloco livre dentro do heap.
static uint8_t *heap_ptr;

/*   Inicialização   */

void memory_init(void){
    heap_ptr = heap_base;
}

/*   Alocador bump   */

void *kmalloc(uint64_t size)
{
    if (size == 0)
        return 0;

    /* Alinhamento para 8 bytes */
    size = (size + 7) & ~7ULL;

    //se a memoria passar do maximo para.
    if (heap_ptr + size > heap_end)
        return 0;   // out of memory

    void *ptr = heap_ptr;
    heap_ptr += size;

    return ptr;
}

/*   Free mínimo   */

void kfree(void *ptr){
    /* Implementação mínima: não faz nada */
    (void)ptr;
}

/*   Estatísticas   */

uint64_t memory_used(void){
    return (uint64_t)(heap_ptr - heap_base);
}

uint64_t memory_free(void){
    return (uint64_t)(heap_end - heap_ptr);
}


//TESTES
uint8_t where_heap_ptr(void){
    return heap_ptr;
}