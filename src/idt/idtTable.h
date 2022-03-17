#pragma once

#include "../common/common.h"
#include "../busComm/portComm.h"
#include "idtEntry.h"



extern void* isr_stub_table[];


#pragma pack(push,1)
typedef struct{
    uint16_t size;
    uint64_t offset;
} __attribute__ ((packed)) idtr;
#pragma pack(pop)


class idtTable{
    private:
        idt_entry idt_table[256];
        idtEntry* idt_object_table;
    

    private:
        void createIDTR(idtr* idt_ptr);

        void loadIDT();

        void initIDT();

        void activateIdt();

        void activatePIC(int master_offset, int slave_offset);

        void convertObjectToStruct();


    public:
        idtTable();

        void createNewEntry(unsigned int int_vector, uint64_t isr_addr, uint8_t attributes, uint16_t cs_selector, uint8_t ist);
        
        void activateInterrupts();

        void unmaskInterrupt(unsigned int int_vector);

        void maskInterrupt(unsigned int int_vector);
};
