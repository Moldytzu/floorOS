#include "pageMapEntry.h"
#include "../boot/videoDriver.h"

pageMapEntry::pageMapEntry(){
    entry = 0;
    entry = 0;
    return;
}


void pageMapEntry::setAddress(uint64_t addr){
    entry = addr;
}

uint64_t pageMapEntry::getAddress(){
    return entry & ~0xfff;
}


void pageMapEntry::setFlag(pageFlags_t flag, flagState_t state){
    uint64_t bitmask = (uint64_t)1 << flag;

    if(state == flagState_t::on){
        entry |= bitmask;
    }

    else if(state == flagState_t::off){
        entry &= ~bitmask;
    }
}


uint64_t pageMapEntry::getFlag(pageFlags_t flag){
    uint64_t bitmask = (uint64_t)1 << flag;

    uint64_t flag_state = (entry & bitmask);
    return flag_state;
}




