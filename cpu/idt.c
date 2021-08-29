#include "cpu/idt.h"

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt_gate(int n, uint32_t handler) {
    idt[n].base_lo = handler & 0xFFFF;
    idt[n].sel = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E;    /* IRQ is present, 32-bit IRQ gate */
    idt[n].base_hi = (handler >> 16) & 0xFFFF;
}

void set_idt() {
    idt_reg.base = (uint32_t) &idt;
    idt_reg.limit = (IDT_ENTRIES * sizeof(idt_gate_t)) - 1;
    /* Don't make the mistake of loading &idt -- always load &idt_reg */
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}
