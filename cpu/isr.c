#include "isr.h"
#include "idt.h"
#include "../drivers/ports.h"
#include "../drivers/vga.h"

#include <stddef.h>
#include <stdint.h>

isr_t irq_handlers[256];

void isr_install(void) {
    remap_pic(32, 40);

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

    set_idt_gate(32, (uint32_t)irq0);
    set_idt_gate(33, (uint32_t)irq1);
    set_idt_gate(34, (uint32_t)irq2);
    set_idt_gate(35, (uint32_t)irq3);
    set_idt_gate(36, (uint32_t)irq4);
    set_idt_gate(37, (uint32_t)irq5);
    set_idt_gate(38, (uint32_t)irq6);
    set_idt_gate(39, (uint32_t)irq7);
    set_idt_gate(40, (uint32_t)irq8);
    set_idt_gate(41, (uint32_t)irq9);
    set_idt_gate(42, (uint32_t)irq10);
    set_idt_gate(43, (uint32_t)irq11);
    set_idt_gate(44, (uint32_t)irq12);
    set_idt_gate(45, (uint32_t)irq13);
    set_idt_gate(46, (uint32_t)irq14);
    set_idt_gate(47, (uint32_t)irq15);

    set_idt(); // Load with ASM
}


void remap_pic(uint8_t master_offset, uint8_t slave_offset) {
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);
    port_byte_out(0x21, master_offset);
    port_byte_out(0xA1, slave_offset);
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0);
}

void exception_handler(registers_t r) {
    vga_print_str("received interrupt: ", -1, -1);
    vga_print_dec(r.int_no);
}

void register_irq_handler(uint8_t n, isr_t handler) {
    irq_handlers[n] = handler;
}

void common_irq_handler(registers_t r) {
    /* After every interrupt we need to send an EOI to the PICs
     * or they will not send another interrupt again */
    if (r.int_no >= 40)
        port_byte_out(0xA0, 0x20); /* Ack slave */
    port_byte_out(0x20, 0x20); /* Ack master regardless */

    /* TODO: handle spurious irq */

    if (irq_handlers[r.int_no] != NULL) {
        isr_t handler = irq_handlers[r.int_no];
        handler(r);
    }
}
