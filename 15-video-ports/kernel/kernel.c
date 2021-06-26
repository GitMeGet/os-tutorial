#include "../drivers/ports.h"
#include "../drivers/vga.h"

void main() {
    vga_print_str("I'm gonna write this very long string to the last row, plus a newline.", 0, 24);
    vga_print_str("\n", -1, -1);
    vga_print_str("Another line of text...", -1, -1);
}
