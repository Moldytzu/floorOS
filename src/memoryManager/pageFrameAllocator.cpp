#include "pageFrameAllocator.h"


void __stack_chk_fail(void){
    return;
}


pageFrameAllocator::pageFrameAllocator(stivale2_struct_tag_memmap* memmap) : memmap(memmap) {
    initAllocator();
}


void pageFrameAllocator::initAllocator(){
    int free_pages = 0;
    for(uint64_t i=0; i < memmap->entries; i++){
        if(memmap->memmap[i].type == STIVALE2_MMAP_USABLE) {
            devideMemoryArea(memmap->memmap[i].base, memmap->memmap[i].length, 0x1000);
        }
    }
}



void pageFrameAllocator::devideMemoryArea(uintptr_t mem_ptr, unsigned int size, unsigned int chunk_size){
    unsigned int num_of_pages = size / chunk_size;
    for(int i=0; i < num_of_pages; i++){
        bitmap_size++;
        page_frame_bitmap[bitmap_size].ptr = mem_ptr + chunk_size * i;
        page_frame_bitmap[bitmap_size].state = memory_chunk_state::useable;
    }
}


int pageFrameAllocator::getPageFrame(uint64_t* page_pointer){
    for(uint64_t i=0; i<bitmap_size; i++){
        if(page_frame_bitmap[i].state == memory_chunk_state::useable){
            *page_pointer = page_frame_bitmap[i].ptr;
            page_frame_bitmap[i].state = memory_chunk_state::reserved;
            return i;
        }
    }
    return -1;
}


void pageFrameAllocator::freePageFrame(uint64_t page_index){
    page_frame_bitmap[page_index].state = memory_chunk_state::useable;
}