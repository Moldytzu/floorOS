#pragma once

#include "../common/common.h"
#include "../boot/videoDriver.h"


#pragma pack(push,1)
typedef struct Entry{
    uint16_t isr_addr_low;
    uint16_t cs_selector;
    uint8_t ist;
    uint8_t attributes;
    uint16_t isr_addr_mid;
    uint32_t isr_addr_high;
    uint32_t reserved;
} __attribute__ ((packed)) idt_entry;
#pragma pack(pop)

class idtEntry{
    private:
        uint16_t isr_addr_low;
        uint16_t cs_selector;
        uint8_t ist;
        uint8_t attributes;
        uint16_t isr_addr_mid;
        uint32_t isr_addr_high;
        uint32_t reserved;


    private:
        void initIsrAddr(uint64_t isr_addr);


    public:
        idtEntry();

        void initEntry(uint64_t isr_addr, uint16_t cs_selector, uint8_t attributes, uint8_t ist);

        void getEntry(idt_entry* entry_buff);
};