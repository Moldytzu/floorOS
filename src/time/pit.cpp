#include "pit.h"
#include "../boot/videoDriver.h"


uint8_t portRead(int portNumber){
    uint8_t buff;
    __asm__ volatile("inb %1, %0" : "=a" (buff) : "Nd" (portNumber));
    return buff;
}


uint8_t portWrite(int portNumber, uint8_t data){
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (portNumber));
    return 0;
}


static int timer = 0;


int read_count(){
    portWrite(0x40, 0);

    int count = portRead(0x43);
    count |= portRead(0x43) << 8;
}


int write_count(unsigned short count){
    portWrite(0x40, count & 0xFF);
    portWrite(0x40, count & 0xFF00);
    return 0;
}


void init_pit(){
    int count = 1193180 / 128;
    portWrite(0x43, 0x36);
    portWrite(0x40, (count & 0xFF));
    portWrite(0x40, (count >> 8) & 0xFF);
}


void timer_callback(){
    timer++;
    if(timer == 1){
        printf("pit", 1);
    }
    printf(itoa(timer, ""), 10);
    portWrite(0x20, 0x20);
    return;
}