#include "../drivers/ports.h"
#include "../drivers/vga.h"

void main() {
    vga_print_char('H', -1, -1, 0);
    vga_print_str("ello world", 0, 20);
}
