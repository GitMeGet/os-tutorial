#include "../cpu/isr.h"
#include "../drivers/ports.h"
#include "../drivers/vga.h"

void main() {
    vga_print("I'm gonna write this very long string to the last row, plus a newline.");
    vga_print("\n");
    vga_print("Another line of text...");

    isr_install();
    asm volatile ("int $0x1E");
}
