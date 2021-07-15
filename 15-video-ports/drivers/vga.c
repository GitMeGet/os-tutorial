#include "ports.h"
#include "../kernel/util.h"
#include "vga.h"

#include <stdint.h>

/* 2,147,483,647 -> 10 characters */
#define INT32_MAX_DEC_CHAR 10

static
uint16_t vga_get_mem_offset(int col, int row) {
    return ((row * VGA_TEXT_MODE_MAX_COLS) + col) * VGA_CHAR_CELL_NUM_BYTES;
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
    return offset * VGA_CHAR_CELL_NUM_BYTES;
}

static
void vga_set_cursor(uint16_t offset) {
    offset /= VGA_CHAR_CELL_NUM_BYTES;

    port_byte_out(VGA_REG_CTRL, 0x0F);
    port_byte_out(VGA_REG_DATA, offset & 0xFF);

    port_byte_out(VGA_REG_CTRL, 0x0E);
    port_byte_out(VGA_REG_DATA, (offset >> 8) & 0xFF);
}

static
uint16_t vga_handle_scrolling(uint16_t offset) {
    /* If the offset is within screen, don't modify */
    if (offset < vga_get_mem_offset(VGA_TEXT_MODE_MAX_COLS-1,
                                    VGA_TEXT_MODE_MAX_ROWS-1)) {
        return offset;
    }

    /* TODO: handle scrolling back more than one row */
    /* Shift rows back by one */
    for (int i = 1; i < VGA_TEXT_MODE_MAX_ROWS; i++) {
        memory_copy((uint8_t*) (vga_get_mem_offset(0, i-1) + VGA_START_ADDR),
                    (uint8_t*) (vga_get_mem_offset(0, i) + VGA_START_ADDR),
                    VGA_TEXT_MODE_MAX_COLS * VGA_CHAR_CELL_NUM_BYTES);
    }

    /* Blank last row */
    vga_clear_row(VGA_TEXT_MODE_MAX_ROWS-1);

    /* Move cursor back to start of row */
    offset = vga_get_mem_offset(0, VGA_TEXT_MODE_MAX_ROWS-1);

    return offset;
}

void vga_print_char(char c, int col, int row, uint8_t attr) {
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
        int rows = offset / (VGA_TEXT_MODE_MAX_COLS * VGA_CHAR_CELL_NUM_BYTES);
        offset = vga_get_mem_offset(VGA_TEXT_MODE_MAX_COLS-1, rows);
    /* Else, write to video memory at calculated offset */
    } else {
        vid_mem[offset] = c;
        vid_mem[offset+1] = attr;
    }

    /* Update the offset to the next character cell */
    offset += VGA_CHAR_CELL_NUM_BYTES;

    /* Scroll if bottom of the screen reached */
    offset = vga_handle_scrolling(offset);

    /* TODO: Don't need to update cursor every time a new char is displayed.
     *       Faster to only update it after printing an entire string
     */
    vga_set_cursor(offset);
}

void vga_print_str(const char* str, int col, int row) {
    /* Set new cursor pos, if not printing from current cursor */
    if (row >= 0 && col >= 0)
        vga_set_cursor(vga_get_mem_offset(col, row));

    /* Print char by char, starting from new cursor pos */
    while (*str != 0) {
        vga_print_char(*str, -1, -1, 0);
        str++;
    }
}

void vga_print(const char* str) {
    vga_print_str(str, -1, -1);
}

void vga_print_dec(int32_t dec) {
    uint8_t str[INT32_MAX_DEC_CHAR];
    uint8_t len;

    int_to_ascii(dec, str, &len);

    while(len > 0) {
        vga_print_char(str[--len], -1, -1, 0);
    }
}

void vga_clear_row(int row) {
    uint8_t* vga_mem_ptr = (uint8_t*) (vga_get_mem_offset(0, row) + VGA_START_ADDR);

    for (int i = 0; i < VGA_TEXT_MODE_MAX_COLS; i++) {
        *vga_mem_ptr = 0;
        /* Skip attr byte, else cursor won't be visible */
        vga_mem_ptr += VGA_CHAR_CELL_NUM_BYTES;
    }
}

void vga_clear_screen(void) {
    for (int row = 0; row < VGA_TEXT_MODE_MAX_ROWS; row++) {
        vga_clear_row(row);
    }

    vga_set_cursor(vga_get_mem_offset(0,0));
}
