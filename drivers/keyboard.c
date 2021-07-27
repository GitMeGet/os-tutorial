#include "../cpu/isr.h"
#include "../kernel/util.h"
#include "keyboard.h"
#include "ports.h"
#include "vga.h"

char sc_to_ascii[256] = {
'\0',   /* 0x00: key detection error or internal buffer overrun */
'\0',   /* 0x01: escape */
/* 0x02 */
'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
'\b',   /* 0x0e: backspace */
'\t',   /* 0x0f: tab */
/* 0x10 */
'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
'\n',   /* 0x1c: enter */
'\0',   /* 0x1d: left ctrl */
/* 0x1e */
'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
'\0',   /* 0x2a: left shift */
'\\',   /* 0x2b: backslash */
/* 0x2c */
'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
'\0',   /* 0x36: right shift */
'*',    /* 0x37: keypad * */
'\0',   /* 0x38: left alt */
' ',    /* 0x39: space */
};

uint32_t keybuf_start = 0;
uint32_t keybuf_end = 0;
uint32_t keybuf_scanned = 0;
char keybuf[KEYBUF_MAX];

static
void keyboard_irq_handler(registers_t regs) {
    uint8_t scan_code = port_byte_in(0x60);
    char ascii_char = sc_to_ascii[scan_code];

    /* ignore key up */
    if (scan_code & 0x80)
        return;

    vga_print_char(ascii_char, -1, -1, 0);

    /* append to keybuf */
    keybuf[keybuf_end] = ascii_char;
    keybuf_end = (keybuf_end + 1) % KEYBUF_MAX;
}

void keyboard_init(void) {
    register_irq_handler(33, keyboard_irq_handler);
}

void keyboard_get_cmd(char* str, uint32_t* len) {
    uint8_t found = 0;
    uint32_t nl_idx;

    /* TODO: handle case of keybuf_end < keybuf_start */

    for (nl_idx = keybuf_scanned; nl_idx < keybuf_end; nl_idx++) {
        if (keybuf[nl_idx] == '\n') {
            found = 1;
            break;
        }
    }

    if (found) {
        *len = nl_idx - keybuf_start + 1;
        memory_copy(str, &(keybuf[keybuf_start]), *len);
        keybuf_start = (nl_idx + 1) % KEYBUF_MAX;
        keybuf_scanned = keybuf_start;
    } else {
        keybuf_scanned = keybuf_end;
    }
}

