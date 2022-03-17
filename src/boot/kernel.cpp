#include "../common/stivale2.h"
#include "videoDriver.h"
#include "../memoryManager/pageFrameAllocator.h"
#include "../memoryManager/paging.h"
#include "../idt/idtTable.h"
#include "../time/pit.h"
#include "../idt/isr.h"

static uint8_t stack[8192];
extern "C" void isr_divide_by_zero();
extern "C" void isr_pit_irq();

static struct stivale2_header_tag_any_video any_video_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_ANY_VIDEO_ID,
        .next = (uint64_t)0 
    },

    .preference = 1
};


__attribute__((section(".stivale2hdr"), used))
static struct stivale2_header stivale_hdr = {
    .entry_point = 0,
    .stack = (uintptr_t)stack + sizeof(stack),
    .flags = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4),
    .tags = (uintptr_t)&any_video_hdr_tag
};


void* stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id) {
    struct stivale2_tag *current_tag = (void*)stivale2_struct->tags;
    for (;;) {
        if (current_tag == NULL) {
            return NULL;
        }
 
        if (current_tag->identifier == id) {
            return current_tag;
        }
 
        current_tag = (void*)current_tag->next;
    }
}


void _start(struct stivale2_struct *stivale2_struct) {
    //printf("hello, stivale!", 10);
    //idtTable idt = idtTable();
    struct stivale2_struct_tag_memmap* memmap = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);
    struct stivale2_struct_tag_kernel_base_address* base_address = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_KERNEL_BASE_ADDRESS_ID);
    struct stivale2_struct_tag_pmrs* pmrs = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_PMRS_ID);
    //pageFrameAllocator allocator = pageFrameAllocator(memmap);
    //paging pageManager = paging(&allocator, base_address, pmrs);
    //uintptr_t ptr1;

    /*for(auto i = 0; i < memmap->entries; i++){
        for(auto j = 0; j < memmap->memmap[i].length; j += 4096){
            uint64_t addr = memmap->memmap[i].base + j;
            pageManager.mapPage(addr, 0x2000000 + addr - 0xffffffff80000000, 3);
        }
    }
    

    for(auto i = 0; i < pmrs->entries; i++){
        for(auto j = 0; j < pmrs->pmrs[i].length; j += 4096){
            uint64_t addr = pmrs->pmrs[i].base + j;
            pageManager.mapPage(addr,0x2000000 + addr - 0xffffffff80000000, 3);
        }
    }

    for(auto i = 0; i < 0x10000000; i += 4096){
        pageManager.mapPage(0xffffffff80000000 + i, 0x2000000 + i, 3);
    }*/

    //printf(itoa(memmap->entries, ""), 21);

    idtTable idt = idtTable();
    idt.createNewEntry(0, (uint64_t)isr_divide_by_zero, 0x8e, 0x28, 0);
    idt.createNewEntry(0x20, (uint64_t)isr_pit_irq, 0x8e, 0x28, 0);
    //write_count(10);
    //read_count();
    init_pit();
    write_count(10);


    idt.activateInterrupts();
    int a = 1/0;
    asm("int $0");
    //pageManager.initCR3();
    while(1);
    asm("hlt");
}

