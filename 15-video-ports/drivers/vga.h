#include <stdint.h>

#define VGA_START_ADDR           0xb8000
#define VGA_TEXT_MODE_MAX_ROWS   25
#define VGA_TEXT_MODE_MAX_COLS   80
/* Character cell consists of character byte and attribute byte */
#define VGA_CHAR_CELL_NUM_BYTES  2
#define VGA_COLOR_WHITE_ON_BLACK 0x0f

#define VGA_REG_CTRL             0x3d4
#define VGA_REG_DATA             0x3d5

void vga_print_char(uint8_t c, int col, int row, uint8_t attr);

/* Prints a null-terminated string
 *
 * @param col: starting column
 * @param row: starting row
 *
 * @return void
 */
void vga_print_str(uint8_t* str, int col, int row);

/* Calls vga_print_str() at current cursor */
void vga_print(uint8_t* str);

void vga_print_dec(int32_t dec);

void vga_clear_row(int row);

void vga_clear_screen(void);
