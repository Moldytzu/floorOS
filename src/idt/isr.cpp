#include "isr.h"
#include "../boot/videoDriver.h"

void dummy_isr(){
    printf("interrupt", 7);
    return;
}

void dummy_page_fault(){
    printf("page fault", 7);
    return;
}




