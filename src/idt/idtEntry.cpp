#include "idtEntry.h"

idtEntry::idtEntry(uint64_t isr_addr, uint16_t cs_selector, uint8_t attributes, uint8_t ist) : cs_selector(cs_selector), attributes(attributes), ist(ist){
   initEntry(isr_addr);
}


void idtEntry::initEntry(uint64_t isr_addr){
    printf(itoa(cs_selector, ""), 19);
    initIsrAddr(isr_addr);
    this->cs_selector = cs_selector;
    this->attributes = attributes;
    this->ist = ist;
    this->reserved = 0;
}


void idtEntry::initIsrAddr(uint64_t isr_addr){
    this->isr_addr_low = isr_addr & 0xFFFF;
    this->isr_addr_mid = (isr_addr >> 16) & 0xFFFF;
    this->isr_addr_high = (isr_addr >> 32) & 0xFFFFFFFF;
}


void idtEntry::getEntry(idt_entry* entry_buff){
    entry_buff->attributes = attributes;
    entry_buff->cs_selector = cs_selector;
    entry_buff->ist = ist;
    entry_buff->reserved = reserved;
    entry_buff->isr_addr_low = isr_addr_low;
    entry_buff->isr_addr_mid = isr_addr_mid;
    entry_buff->isr_addr_high = isr_addr_high;
}

