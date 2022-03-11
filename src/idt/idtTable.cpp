#include "idtTable.h"
#include "isr.h"

extern void* isr_stub_table[];
extern "C" void isr_no_err_stub_257();


idtTable::idtTable(){
    initIDT();
}


void idtTable::initIDT(){
    for(int i = 0; i<=31; i++){
        createNewEntry(i, (uint64_t)isr_stub_table[i], 0x8e, 0x08, 0);
    }
}


void idtTable::createNewEntry(unsigned int int_vector, uint64_t isr_addr, uint8_t attributes, uint16_t cs_selector, uint8_t ist){
    if (int_vector > 255){
        return;
    }

    idt_object_table[int_vector].initEntry(isr_addr, cs_selector, attributes, ist);
}


void idtTable::convertObjectToStruct(){
    for(int i=0; i<=255; i++){
        idt_object_table[i].getEntry(&idt_table[i]);
    }
}


void idtTable::createIDTR(idtr* idt_ptr){
    idt_ptr->size = (sizeof(idt_entry) * 256) - 1;
    idt_ptr->offset = (uint64_t)idt_table;
}


void idtTable::loadIDT(){
    idtr idt_ptr;
    createIDTR(&idt_ptr);
    __asm__ volatile("lidt %0" : : "m"(idt_ptr) : "memory");
}


void idtTable::activateInterrupts(){
    convertObjectToStruct();
    loadIDT();
    activatePIC(0, 0);
    asm("sti");
}


void idtTable::activatePIC(int master_offset, int slave_offset){
    portComm masterCommand = portComm(0x20, 1);
    portComm masterData = portComm(0x21, 1);
    portComm slaveCommand = portComm(0xA0, 1);
    portComm slaveData = portComm(0xA1, 1);

    unsigned char master_mask = masterData.highSpeedRead();
    unsigned char slave_mask = slaveData.highSpeedRead();

    masterCommand.highSpeedWrite(0x11);
    slaveCommand.highSpeedWrite(0x11);

    masterData.highSpeedWrite(master_offset);
    slaveData.highSpeedWrite(slave_offset);

    masterData.highSpeedWrite(4);
    slaveData.highSpeedWrite(2);

    masterData.highSpeedWrite(0x01);
    slaveData.highSpeedWrite(0x01);

    masterData.highSpeedWrite(master_mask);
    slaveData.highSpeedWrite(slave_mask);
}


