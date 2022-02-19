#include "pageMapEntry.h"


pageMapEntry::pageMapEntry(){
    return;
}


void pageMapEntry::setAddress(uint64_t addr){
    entry = addr << 12;
}


uint64_t pageMapEntry::getAddress(){
    return entry << 12;
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


int pageMapEntry::getFlag(pageFlags_t flag){
    uint64_t bitmask = (uint64_t)1 << flag;
    return entry & bitmask;
}
