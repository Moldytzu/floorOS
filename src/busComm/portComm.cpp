#include "portComm.h"


uint8_t portComm::highSpeedRead(){
    if(speedType != 1){
        return -1;
    }

    uint8_t buff;
    __asm__ volatile("inb %1, %0" : "=a" (buff) : "Nd" (portNumber));
    return buff;
}

/*uint8_t Port8Bit::lowSpeedRead(){
    if(speedType != 0){
        return -1;
    }

    uint8_t buff;
    __asm__ volatile("inb %1, %0" : "=a" (buff) : "Nd" (portNumber));
    return buff;
}*/

uint8_t portComm::highSpeedWrite(uint8_t data){
    if(speedType != 1){
        return -1;
    }

    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (portNumber));
    return 0;
}

/*uint8_t Port8Bit::lowSpeedWrite(uint8_t data){
    if(speedType != 0){
        return -1;
    }

    __asm__ volatile("outb %1, %0" : : "a" (data), "Nd" (portNumber));
    return 0;
}*/

