#include "../common/common.h"
#include "../common/stivale2.h"
#include "../boot/videoDriver.h"


void __stack_chk_fail(void);


typedef enum memory_chunk_state{
    useable,
    reserved
} memory_chunk_state;


typedef struct memory_chunk{
    memory_chunk_state state;
    uintptr_t ptr;
} memory_chunk;


class pageFrameAllocator{
    private:
        memory_chunk* page_frame_bitmap;
        uint64_t bitmap_size;
        uint8_t* last_occupied_page;
        stivale2_struct_tag_memmap* memmap;
    

    private:

        /**
         * initialize the pageFramAllocator, checking on the memory map which memory pages are occupied and which are usable
         * and initialize the bitmap by this information.
         * @param: memmap struct
         * @return: none
         */
        void initAllocator();

        /**
         * devide the given memory area into chunks and stores them in the bitmap
         * @param: a pointer to the start of the memory area to be devided into chuncks
         * @param: the size of the memory area
         * @param: the size of the chunks
         * @return: None
         */
        void devideMemoryArea(uintptr_t mem_ptr, unsigned int size, unsigned int chunk_size);


    public:

        /**
         * pageFrameAllocator constructor, should call the pageFrameAllocatorInit() function
         * @param: memory map from the bootloader(limine with stivale protocol)
         * @return none
         */
        pageFrameAllocator(stivale2_struct_tag_memmap* memmap);


        /**
         * search for avaible and usable memory page in page_fram_bitmap and returning pointer to the start of the page.
         * Sets the last_occupied_page pointer to the index of the page in the bitmap
         * sets the pointer in the parameter to the start of the memory page
         * @param:  pointer that will be set to the start of the memory page
         * @return: the index of the memory page frames in the bitmap(will be used to free the memory)
         * return -1 if all the memory page frames are occupied
         */
        int getPageFrame(uint64_t* page_pointer);


        /**
         * set the bitmap entry at the index that have been givven to the function to usable
         * @param: page_index the index of the page to free
         * @return: none
         */
        void freePageFrame(uint64_t page_index); //unused for now
};