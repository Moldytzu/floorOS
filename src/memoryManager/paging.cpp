#include "paging.h"


void *memset(void *bufptr, int value, unsigned int size)
{
    unsigned char *buf = (unsigned char *)bufptr;
    unsigned int i;
    for (i = 0; i < size; i++){
        int b = i;
        buf[b] = (unsigned char)value;
    }

    return bufptr;
}


paging::paging(pageFrameAllocator* pageAllocator, stivale2_struct_tag_kernel_base_address* kernel_base_addr_struct, stivale2_struct_tag_pmrs* pmrs) : pageAllocator(pageAllocator){
    initPaging(kernel_base_addr_struct, pmrs);
}


int paging::initPaging(stivale2_struct_tag_kernel_base_address* kernel_base_addr_struct, stivale2_struct_tag_pmrs* pmrs){
    pageAllocator->getPageFrame(&pml4_phys_addr);
    pml4 = reinterpret_cast<pageMapLevel_t*>(pml4_phys_addr);
    memset((void*)pml4_phys_addr, 0, 0x1000);
    //identityPagingInit(kernel_base_addr_struct, pmrs);
    return 0;
}


void paging::parsePageMapIndexes(uint64_t virtAddr, unsigned int* indexes){    
    for(int i=0; i < 4; i++){
        indexes[i] = ((virtAddr & ((uintptr_t)0x1ff << 39 - 9 * i)) >> 39 - 9 * i);
    }
}


pageMapLevel_t* paging::getNextPagingLevel(pageMapLevel_t* cur_level, unsigned int entry_index){
    if(cur_level->entries[entry_index].getFlag(pageFlags_t::present) == flagState_t::off){
        uint64_t page_ptr;
        pageAllocator->getPageFrame(&page_ptr);
        memset((void*)page_ptr, 0, 0x1000);
        pageMapEntry entry = pageMapEntry();
        entry.setAddress(page_ptr);
        entry.setFlag(pageFlags_t::present, flagState_t::on);
        entry.setFlag(pageFlags_t::read_write, flagState_t::on);
        return reinterpret_cast<pageMapLevel_t*>(page_ptr);
    }
    else{
        auto entry = reinterpret_cast<pageMapLevel_t*>(cur_level->entries[entry_index].getAddress());
        return entry;
    }
}

/*int paging::mapPage(uint64_t virtAddr, uint64_t physAddr){
    unsigned int indexes[4];
    parsePageMapIndexes(virtAddr, indexes);

    pageMapLevel_t* table = reinterpret_cast<pageMapLevel_t*>(pml4_phys_addr);
    for(int i = 0; i < 4; i++){
        auto entry = &table->entries[indexes[i]];
        if(entry->getFlag(pageFlags_t::present) == flagState_t::off){
            uint64_t page_ptr;
            pageAllocator->getPageFrame(&page_ptr);
            entry->setAddress(page_ptr);
            //entry->setFlag(pageFlags_t::present, flagState_t::on);
            //entry->setFlag(pageFlags_t::read_write, flagState_t::on);
            memset((void*)page_ptr, 0, 0x1000);
            table = reinterpret_cast<pageMapLevel_t*>(page_ptr);
        }
        else{
            printf("page!", 6);
            table = reinterpret_cast<pageMapLevel_t*>(entry->getAddress());
        }
    }
    
    
    table->entries[((virtAddr & ((uintptr_t)0x1FF << 12)) >> 12)].setAddress(physAddr);
    //table->entries[((virtAddr & ((uintptr_t)0xFF << 12)) >> 12)].setFlag(pageFlags_t::present, flagState_t::on);
    //table->entries[((virtAddr & ((uintptr_t)0xFF << 12)) >> 12)].setFlag(pageFlags_t::read_write, flagState_t::on);
    return 0;
}*/


int paging::mapPage(uint64_t virtAddr, uint64_t physAddr, uint64_t flags){
    unsigned int indexes[4];
    parsePageMapIndexes(virtAddr, indexes);

    pageMapLevel_t* table = reinterpret_cast<pageMapLevel_t*>(pml4_phys_addr);
    for(int i = 0; i < 3; i++){
        auto entry = &table->entries[indexes[i]];
        if(entry->getFlag(pageFlags_t::present) == flagState_t::off){
            uint64_t page_ptr;
            pageAllocator->getPageFrame(&page_ptr);
            entry->setAddress(page_ptr | flags);
            //entry->setFlag(pageFlags_t::present, flagState_t::on);
            //entry->setFlag(pageFlags_t::read_write, flagState_t::on);
            memset((void*)page_ptr, 0, 0x1000);
            table = reinterpret_cast<pageMapLevel_t*>(page_ptr);
        }
        else{
            table = reinterpret_cast<pageMapLevel_t*>(entry->getAddress());
        }
    }
    
    
    table->entries[((virtAddr & ((uintptr_t)0x1FF << 12)) >> 12)].setAddress(physAddr | 3);
    //table->entries[((virtAddr & ((uintptr_t)0x1FF << 12)) >> 12)].setFlag(pageFlags_t::present, flagState_t::on);
    //table->entries[((virtAddr & ((uintptr_t)0x1FF << 12)) >> 12)].setFlag(pageFlags_t::read_write, flagState_t::on);
    return table->entries[((virtAddr & ((uintptr_t)0x1FF << 12)) >> 12)].getAddress();
}


void paging::identityPagingInit(stivale2_struct_tag_kernel_base_address* kernel_base_address, stivale2_struct_tag_pmrs* pmrs){
    uint64_t kernel_base_phys = kernel_base_address->physical_base_address;
    uint64_t kernel_base_virt = kernel_base_address->virtual_base_address;

    mapPage(kernel_base_phys, kernel_base_virt, 3);
    /*for(uint64_t i = 0; i <= 0x100000000; i += 4096){
        printf("start id mapping", 8);
        mapPage(kernel_base_phys + i, kernel_base_phys + i);
    }*/

    for(uint64_t i = 0; i <= 0x10000000; i += 4096){
        uint64_t addr = 0xffffffff80200000 + i;
        mapPage(addr, addr, 3);
    }

    printf("finish identity paging", 20);
}

int paging::translateAddr(uint64_t virtAddr){
    unsigned int indexes[4];
    parsePageMapIndexes(virtAddr, indexes);

    pageMapLevel_t* table = reinterpret_cast<pageMapLevel_t*>(pml4_phys_addr);
    for(int i = 0; i < 3; i++){
        auto entry = &table->entries[indexes[i]];
        if(entry->getFlag(pageFlags_t::present) == flagState_t::off){
            return -1;
        }
        else{
            printf(itoa(entry->getAddress(), ""), 10+i);
            table = reinterpret_cast<pageMapLevel_t*>(entry->getAddress());
        }
    }
    
    return table->entries[((virtAddr & ((uintptr_t)0x1FF << 12)) >> 12)].getAddress();
}


void paging::initCR3(){
    __asm__ volatile("mov %0, %%cr3" : : "r" (pml4_phys_addr) );
}


//check why the paging is crashing and getting a page fault when load the address to CR3
