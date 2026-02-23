#include <stdint.h>

extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;

extern uint32_t _sbss;
extern uint32_t _ebss;

/* --------------------------------------------------
 * main() prototype
 * -------------------------------------------------- */
extern int main(void);


/* --------------------------------------------------
 * Runtime initialization entry point
 * Called from Reset_Handler (assembly)
 * -------------------------------------------------- */
void runtime_init(void)
{
    uint32_t *src;
    uint32_t *dst;

    /* ----------------------------------------------
     * Copy .data section from FLASH to RAM
     * LMA (_sidata) --> VMA (_sdata)
     * ---------------------------------------------- */
    src = &_sidata;
    dst = &_sdata;

    while (dst < &_edata) {
        *dst++ = *src++;
    }
    
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0U;
    }

    /* ----------------------------------------------
     * Call user entry point
     * ---------------------------------------------- */
    (void)main();

    /* ----------------------------------------------
     * main() should never return
     * ---------------------------------------------- */
    while (1) {
        /* trap */
    }
}