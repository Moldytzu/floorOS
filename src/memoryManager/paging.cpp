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


paging::paging(pageFrameAllocator* pageAllocator) : pageAllocator(pageAllocator){
    initPaging();
}


int paging::initPaging(){
    return 0;
}

int paging::mapPage(uint64_t virtAddr, uint64_t physAddr){
    //map the page directory pointer in the index that is described by the the integer between the 39 bit to  the 47 bit (9 bits : 0x1ff = 9)
    pageMapEntry pdp = pml4.entries[virtAddr >> 39 & 0x1FF];

    pageMapLevel_t* pdpe;
    uint64_t page_alloc;
    //check if its already have been created
    if(pdp.getFlag(present) == 0){
        //if not, allocate a page for the entry and creates an object for it init the object and storing it
        //in the correct index in the page directory pointer
        uint64_t pdp_page;
        pageAllocator->getPageFrame(&pdp_page);
        pageMapEntry entry = pageMapEntry();
        entry.setAddress(pdp_page);
        entry.setFlag(pageFlags_t::present, flagState_t::on);
        entry.setFlag(pageFlags_t::read_write, flagState_t::on);
        memset((void*)pdp_page, 0x1000, 0);
        pml4.entries[virtAddr >> 39 & 0x1FF] = entry;
        pdpe = (pageMapLevel_t*)entry.getAddress();
        delete &entry;
    }

    else{
        //if the page direcotry pointer have already been created, it gets its address and continue in the mapping.
        //the pdpe variable is used all over the function, not only for the pdp, but for all of the different maps.
        //every time we use the pdpe to continue our recursive mapping
        pdpe = (pageMapLevel_t*)pml4.entries[virtAddr >> 39 & 0x1FF].getAddress();
    }

    //the same thing again for the page directory
    if(pdpe->entries[virtAddr >> 30 & 0x1FF].getFlag(pageFlags_t::present) == 0){
        pageAllocator->getPageFrame(&page_alloc);
        pageMapEntry entry = pageMapEntry();
        entry.setAddress(page_alloc);
        entry.setFlag(pageFlags_t::present, flagState_t::on);
        entry.setFlag(pageFlags_t::read_write, flagState_t::on);
        memset((void*)page_alloc, 0x1000, 0);
        pdpe->entries[virtAddr >> 30 & 0x1FF] = entry;
        delete &entry;
    }

    else{
        pdpe = (pageMapLevel_t*)pdpe->entries[virtAddr >> 30 & 0x1FF].getAddress();
    }

    //same thing again for the page table
    if(pdpe->entries[virtAddr >> 21 & 0x1FF].getFlag(pageFlags_t::present) == 0){
        pageAllocator->getPageFrame(&page_alloc);
        pageMapEntry entry = pageMapEntry();
        entry.setAddress(page_alloc);
        entry.setFlag(pageFlags_t::present, flagState_t::on);
        entry.setFlag(pageFlags_t::read_write, flagState_t::on);
        memset((void*)page_alloc, 0x1000, 0);
        pdpe->entries[virtAddr >> 21 & 0x1FF] = entry;
        delete &entry;
    }

    else{
        pdpe = (pageMapLevel_t*)pdpe->entries[virtAddr >> 21 & 0x1FF].getAddress();
    }

    //and now, set the last entry to the physical address.
    pdpe->entries[virtAddr >> 9 & 0x1FF].setAddress(physAddr);
    pageMapEntry entry = pageMapEntry();
    entry.setAddress(physAddr);
    entry.setFlag(pageFlags_t::present, flagState_t::on);
    entry.setFlag(pageFlags_t::read_write, flagState_t::on);
    pdpe->entries[virtAddr >> 9 & 0x1FF] = entry;
    return 0;
}


int paging::translateAddr(uint64_t virtAddr);
    pageMapEntry pdp = pml4.entries[virtAddr >> 39 & 0x1FF];
    pageMapLevel_t* pd = (pageMapLevel_t*)pdp.getAddress();
    pd = (pageMapLevel_t*)pd->entries[virtAddr >> 30 & 0x1FF].getAddress();
    pd = (pageMapLevel_t*)pd->entries[virtAddr >> 21 & 0x1FF].getAddress();
    pdp = pd->entries[virtAddr >> 9 & 0x1FF];
    return pdp.getAddress();
}

