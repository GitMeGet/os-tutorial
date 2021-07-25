#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBUF_MAX (VGA_TEXT_MODE_MAX_CHARS * 2)

void keyboard_init(void);

void keyboard_get_cmd(char* str, uint32_t* len);

#endif /* KEYBOARD_H */
