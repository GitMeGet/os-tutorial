#include "../cpu/isr.h"
#include "keyboard.h"
#include "ports.h"
#include "vga.h"

char sc_to_ascii[56] = {
'\0',   /* key detection error or internal buffer overrun */
'\0',  /* escape */
/* 0x02 */
'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
'\b',   /* 0x0f: backspace */
'\t',   /* tab */
/* 0x10 */
'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
'\n',   /* 0x1c: enter */
'\0',   /* left ctrl */
/* 0x1e */
'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
'\0',   /* 0x2a: left shift */
'\\',   /* 0x2b: backslash */
/* 0x2c */
'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
'\0',   /* 0x36: right shift */
};


static
void keyboard_irq_handler(registers_t regs) {
    uint8_t scan_code = port_byte_in(0x60);
    char ascii_char = sc_to_ascii[scan_code];
    char str[2] = {ascii_char, '\0'};
    vga_print(str);
}

void keyboard_init(void) {
    register_irq_handler(33, keyboard_irq_handler);
}

