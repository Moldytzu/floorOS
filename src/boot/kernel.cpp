#include "../common/stivale2.h"
#include "videoDriver.h"
#include "../memoryManager/pageFrameAllocator.h"
#include "../idt/idtTable.h"

static uint8_t stack[8192];

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
    pageFrameAllocator allocator = pageFrameAllocator(memmap);
    uintptr_t ptr1;
    int page_id = allocator.getPageFrame(&ptr1);
    allocator.freePageFrame(page_id);
    printf(itoa(ptr1, ""), 1);
    allocator.getPageFrame(&ptr1);
    printf(itoa(ptr1, ""), 2);
    allocator.getPageFrame(&ptr1);
    printf(itoa(ptr1, ""), 3);
    while(1);
    asm("hlt");
}

