#pragma once

#include "paging.h"


class pageMapEntry{
    private:
        uint64_t entry;
    
    public:
        pageMapEntry();

        void setAddress(uint64_t addr);

        uint64_t getAddress();

        void setFlag(pageFlags_t flag, flagState_t state);

        int getFlag(pageFlags_t flag);
};