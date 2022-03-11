#pragma once

#include "../common/common.h"


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


class pageMapEntry{
    private:
        uint64_t entry;
    
    public:
        pageMapEntry();

        void setAddress(uint64_t addr);

        uint64_t getAddress();

        void setFlag(pageFlags_t flag, flagState_t state);

        uint64_t getFlag(pageFlags_t flag);
};
