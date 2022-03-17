#include "isr.h"
#include "../boot/videoDriver.h"

void dummy_isr(){
    printf("interrupt", 7);
    return;
}


void divide_by_zero(){
    printf("divide by zero!", 8);
}


/*__attribute__((interrupt))
void dummy_isr_gp(void* dummy, uint64_t error){
    //printf("general protection fault!", 7);
    return;
}*/



