#include "../cpu/isr.h"
#include "keyboard.h"
#include "ports.h"
#include "vga.h"

static
void keyboard_irq_handler(registers_t regs) {
    uint8_t scan_code = port_byte_in(0x60);
    vga_print_dec(scan_code);
}

void keyboard_init(void) {
    register_irq_handler(33, keyboard_irq_handler);
}

