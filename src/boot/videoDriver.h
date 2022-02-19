#define SCREEN_WIDTH 32
#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H

extern "C"

typedef unsigned short int uint16_t;
typedef unsigned char uint8_t;

typedef enum vagColor{
    vga_color_black = 0,
    vga_color_blue = 1,
    vga_color_green = 2,
    vga_color_cyan = 3,
    vga_color_red = 4,
    vga_color_magenta = 5,
    vga_color_brown = 6,
    vga_color_light_gray = 7,
    vga_color_dark_gray = 8,
    vga_color_light_blue = 9,
    vga_color_light_green = 10,
    vga_color_light_cyan = 11,
    vga_color_light_red = 12,
    vga_color_pink = 13,
    vga_color_yellow = 14,
    vga_color_white = 15,
};

uint8_t calculate_entry_color_byte(enum vagColor fc,enum vagColor bc);


uint16_t create_vga_entry(unsigned char character, uint8_t color);

void clear_screen();

int putChar(uint16_t vga_word, int place);

int printf(char *message, int line);

int testPrintf(char* str);

char* itoa(int value, char* buffer);
#endif