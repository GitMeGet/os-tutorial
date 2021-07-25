#include "../cpu/isr.h"
#include "../drivers/keyboard.h"
#include "../drivers/ports.h"
#include "../drivers/vga.h"

char cmdbuf[KEYBUF_MAX];

void handle_user_cmd(void) {
    uint32_t len = 0;
    keyboard_get_cmd(cmdbuf, &len);
    if (len > 0) {
        cmdbuf[len] = '\0';
        vga_print("cmd: ");
        vga_print(cmdbuf);
    }
}

void main() {
    isr_install();
    keyboard_init();
    asm volatile ("int $0x1E");
    asm volatile ("sti");

    while(1) {
        handle_user_cmd();
    }
}
