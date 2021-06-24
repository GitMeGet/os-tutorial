#include "vga.h"
#include "ports.h"

#include <stdint.h>

static
uint16_t vga_get_mem_offset(int col, int row) {
    return ((row * VGA_TEXT_MODE_MAX_COLS) + col) * 2;
}

static
uint16_t vga_get_cursor(void) {
    uint16_t offset;

    port_byte_out(VGA_REG_CTRL, 0x0F);
    offset |= port_byte_in(VGA_REG_DATA);

    port_byte_out(VGA_REG_CTRL, 0x0E);
    offset |= (port_byte_in(VGA_REG_DATA) << 8);

    /* VGA hardware stores character offset.
     * Multiply by 2 to get character cell offset.
     */
    return offset * 2;
}

static
void vga_set_cursor(uint16_t offset) {
    offset /= 2;

    port_byte_out(VGA_REG_CTRL, 0x0F);
    port_byte_out(VGA_REG_DATA, offset & 0xFF);

    port_byte_out(VGA_REG_CTRL, 0x0E);
    port_byte_out(VGA_REG_DATA, (offset >> 8) & 0xFF);
}

static
uint16_t vga_handle_scrolling(uint16_t offset) {
    return offset;
}

void vga_print_char(uint8_t c, int col, int row, uint8_t attr) {
    uint8_t* vid_mem = (uint8_t*) VGA_START_ADDR;

    if (!attr)
        attr = VGA_COLOR_WHITE_ON_BLACK;

    /* Get video memory offset for a specific (row, col) */
    int offset;
    if (row >= 0 && col >= 0) {
        offset = vga_get_mem_offset(col, row);
    } else {
        offset = vga_get_cursor();
    }

   /* If \n, set offset to end of current row, so that
    * it will be advanced to the first col of the next row
    */
    if (c == '\n') {
        int rows = offset / (2*VGA_TEXT_MODE_MAX_COLS);
        offset = vga_get_mem_offset(VGA_TEXT_MODE_MAX_COLS-1, rows);
    /* Else, write to video memory at calculated offset */
    } else {
        vid_mem[offset] = c;
        vid_mem[offset+1] = attr;
    }

    /* Update the offset to the next character cell */
    offset += 2;

    /* Scroll if bottom of the screen reached */
    offset = vga_handle_scrolling(offset);

    /* TODO: Don't need to update cursor every time a new char is displayed.
     *       Faster to only update it after printing an entire string
     */
    vga_set_cursor(offset);
}

void vga_print_str(uint8_t* str, int col, int row) {
    if (row >= 0 && col >= 0)
        vga_set_cursor(vga_get_mem_offset(col, row));

    while (*str != 0) {
        vga_print_char(*str, -1, -1, 0);
        str++;
    }
}
