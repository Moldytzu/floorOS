#pragma once

#include "../common/common.h"
#include "pageFrameAllocator.h"
#include "memory.h"
#include "pageMapEntry.h"


typedef enum pageFlags{
    present,
    read_write,
    user_supervisor,
    write_through,
    cache_disable,
    accessed,
    larger_pages = 7,
    custom0 = 9,
    cutsom1 = 10,
    custom2 = 11,
    NX = 63
} pageFlags_t;


typedef enum flagState{
    off,
    on
} flagState_t;


typedef struct pageMapLevel{
    pageMapEntry entries[512];
} __attribute__((aligned(0x1000))) pageMapLevel_t;


class paging{
    private:
        pageFrameAllocator* pageAllocator;
        pageMapLevel_t pml4;

    private:
        int initPaging();


    public:
        paging(pageFrameAllocator* pageAllocator);


        int mapPage(uint64_t virtAddr, uint64_t physAddr);


        int translateAddr(uint64_t virtAddr);
};