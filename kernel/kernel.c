#include "../cpu/isr.h"
#include "../drivers/keyboard.h"
#include "../drivers/ata.h"
#include "../drivers/ports.h"
#include "../drivers/timer.h"
#include "../drivers/vga.h"
#include "util.h"

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

void test_ata() {
    if (ata_identify()) {
        vga_print("ata_identify() failed\n");
    }

    vga_print("start\n");
    uint16_t dest[512];

    ata_read_sectors(dest, 0, 1);
    for (int i = 0; i < 5; i++) {
        printf("%d\n", dest[i]);
    }

    memory_set((uint8_t *) dest, 0, 512);
    ata_write_sectors(dest, 0, 1);

    ata_read_sectors(dest, 0, 1);
    for (int i = 0; i < 5; i++) {
        printf("%d\n", dest[i]);
    }
    vga_print("done\n");
}

void read_mmap() {
    char *p = (char *)0x7E00;
    printf("%d\n", p[0]);
}

void main() {
    isr_install();
    timer_init();
    keyboard_init();
    asm volatile ("sti");

//    test_ata();
    read_mmap();

    while(1) {
        handle_user_cmd();
    }
}
