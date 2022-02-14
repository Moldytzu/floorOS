#include "videoDriver.h"


uint8_t calculate_entry_color_byte(enum vagColor fc,enum vagColor bc){
    return fc | bc << 4;
}


uint16_t create_vga_entry(unsigned char character, uint8_t color){
    return (uint16_t)character | (uint16_t)color << 8;
}

void clear_screen(){
    char *videomem = (char*) 0xb8000;
    uint8_t color = calculate_entry_color_byte(vga_color_white, vga_color_black);
    for (int i = 0; i < (80*25*2);i++){
        videomem[i] = ' ';
        i++;
        videomem[i] = color;
    }
}

int putChar(uint16_t vga_word, int place){
    char *videomem = (char*)0xb8000;
    videomem[place] = vga_word;
    place += 2;
    return place;
}

int printf(char *message, int line){
    int line_num = line;
    char *videomem = (char*)0xb8000;
    int char_place = (line*80*2);
    uint8_t color = calculate_entry_color_byte(vga_color_green, vga_color_black);
    while (*message != 0){
        if ((const char)*message == '\n'){
            line++;
            char_place = (line*80*2);
            message++;
        }
        else{    
            uint16_t vga_word = create_vga_entry(*message,color);
            char_place = putChar(vga_word, char_place);
            message++;
            char_place += 2;
        }
    }
    return char_place;
}


int testPrintf(char* str){
   unsigned short* videomem = (unsigned short*)0xb8000;

   for(int i =0; str[i] != '\0'; i++){
      videomem[i] = (videomem[i] &0xFF00) | str[i];
   }

   return 0;
}

void swap(char *x, char *y){
    char t = *x; *x = *y; *y = t;
}

char* reverse(char *buffer, int i, int j)
{
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }
 
    return buffer;
}
 
char* itoa(int value, char* buffer){
    int n = value;
 
    int i = 0;
    while (n)
    {
        int r = n % 10;
 
        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }
 
        n = n / 10;
    }
 

    if (i == 0) {
        buffer[i++] = '0';
    }

    if (value < 0 && 10 == 10) {
        buffer[i++] = '-';
    }
 
    buffer[i] = '\0';
 
    return reverse(buffer, 0, i - 1);
}
