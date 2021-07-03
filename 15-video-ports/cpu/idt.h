#ifndef IDT_H
#define IDT_H

#include <stdint.h>

/* Segment selectors */
#define KERNEL_CS 0x08
#define IDT_ENTRIES 256

/* Describes an interrupt gate (handler) */
typedef struct
{
    uint16_t base_lo; /* lower 16 bits of addr to jump to when this irq fires */
    uint16_t sel;     /* Kernel segment selector */
    uint8_t  always0; /* This must always be zero */
    /* First byte
     * Bit 7  : "Interrupt is present"
     * Bit 6-5: Privilege level of caller (0=kernel..3=user)
     * Bit 4  : Set to 0 for interrupt gates
     * Bit 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate"
     */
    uint8_t  flags;
    uint16_t base_hi; /* upper 16 bits of the address to jump to */
} __attribute__((packed)) idt_gate_t;

/* A pointer to the array of interrupt handlers.
 * Assembly instruction 'lidt' will read it */
typedef struct {
    uint16_t limit;   /* idt_num_bytes - 1 */
    uint32_t base;    /* addr of 1st element in idt array */
} __attribute__((packed)) idt_register_t;

void set_idt_gate(int n, uint32_t handler);
void set_idt();

#endif /* IDT_H */
