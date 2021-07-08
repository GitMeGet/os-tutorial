#include "isr.h"
#include "idt.h"
#include "../drivers/vga.h"

#include <stdint.h>

void isr_install() {
    set_idt_gate(0, (uint32_t)exception0);
    set_idt_gate(1, (uint32_t)exception1);
    set_idt_gate(2, (uint32_t)exception2);
    set_idt_gate(3, (uint32_t)exception3);
    set_idt_gate(4, (uint32_t)exception4);
    set_idt_gate(5, (uint32_t)exception5);
    set_idt_gate(6, (uint32_t)exception6);
    set_idt_gate(7, (uint32_t)exception7);
    set_idt_gate(8, (uint32_t)exception8);
    set_idt_gate(9, (uint32_t)exception9);
    set_idt_gate(10, (uint32_t)exception10);
    set_idt_gate(11, (uint32_t)exception11);
    set_idt_gate(12, (uint32_t)exception12);
    set_idt_gate(13, (uint32_t)exception13);
    set_idt_gate(14, (uint32_t)exception14);
    set_idt_gate(15, (uint32_t)exception15);
    set_idt_gate(16, (uint32_t)exception16);
    set_idt_gate(17, (uint32_t)exception17);
    set_idt_gate(18, (uint32_t)exception18);
    set_idt_gate(19, (uint32_t)exception19);
    set_idt_gate(20, (uint32_t)exception20);
    set_idt_gate(21, (uint32_t)exception21);
    set_idt_gate(22, (uint32_t)exception22);
    set_idt_gate(23, (uint32_t)exception23);
    set_idt_gate(24, (uint32_t)exception24);
    set_idt_gate(25, (uint32_t)exception25);
    set_idt_gate(26, (uint32_t)exception26);
    set_idt_gate(27, (uint32_t)exception27);
    set_idt_gate(28, (uint32_t)exception28);
    set_idt_gate(29, (uint32_t)exception29);
    set_idt_gate(30, (uint32_t)exception30);
    set_idt_gate(31, (uint32_t)exception31);

    set_idt(); // Load with ASM
}

void exception_handler(registers_t r) {
    vga_print_str("received interrupt: ", -1, -1);
    vga_print_dec(r.int_no);
}
