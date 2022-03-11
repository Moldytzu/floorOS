#pragma once

#include "../common/common.h"
#include "pageFrameAllocator.h"
#include "memory.h"
#include "pageMapEntry.h"
#include "../common/stivale2.h"


typedef struct pageMapLevel{
    pageMapEntry entries[512];
} __attribute__((aligned(0x1000))) pageMapLevel_t;


typedef struct pageMapIndexes{
    unsigned int pml4_index;
    unsigned int pdp_index;
    unsigned int pd_index;
    unsigned int pt_index;
} pageMapIndexes_t;


class paging{
    private:
        pageFrameAllocator* pageAllocator;
        uint64_t pml4_phys_addr;
        pageMapLevel_t* pml4;


    private:
        int initPaging(stivale2_struct_tag_kernel_base_address* kernel_start_address, stivale2_struct_tag_pmrs* pmrs);

        void identityPagingInit(stivale2_struct_tag_kernel_base_address* kernel_base_address, stivale2_struct_tag_pmrs* pmrs);

        void parsePageMapIndexes(uint64_t virtAddr, unsigned int* indexes);

        pageMapLevel_t* getNextPagingLevel(pageMapLevel_t* cur_level , unsigned int entry_index);

    public:
        paging(pageFrameAllocator* pageAllocator, stivale2_struct_tag_kernel_base_address* kernel_base_addr_struct, stivale2_struct_tag_pmrs* pmrs);

        int mapPage(uint64_t virtAddr, uint64_t physAddr, uint64_t flags);

        int translateAddr(uint64_t virtAddr);

        void initCR3();
};
