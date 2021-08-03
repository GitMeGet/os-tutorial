#include "../cpu/isr.h"
#include "../drivers/keyboard.h"
#include "../drivers/ata.h"
#include "../drivers/ports.h"
#include "../drivers/timer.h"
#include "../drivers/vga.h"

char cmdbuf[KEYBUF_MAX];

void handle_user_cmd(void) {
    uint32_t len = 0;
    int64_t timer_sec;

    timer_sec = timer_get_elapsed_ms();
    keyboard_get_cmd(cmdbuf, &len);
    if (len > 0) {
        cmdbuf[len] = '\0';
        vga_print("time: ");
        vga_print_dec(timer_sec);
        vga_print(" cmd: ");
        vga_print(cmdbuf);
    }
}

void main() {
    isr_install();
    timer_init();
    keyboard_init();
    asm volatile ("sti");

    if (ata_identify()) {
        vga_print("ata_identify() failed\n");
    }

    while(1) {
        handle_user_cmd();
    }
}
