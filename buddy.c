#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

//helps to get both pointers to the a fitting free block and the last free block before that 
struct blocks{
  uint8_t * first_free;
  uint8_t * second_free ;

};

// function declarations
void *malloc(size_t);
void free(void *);
void dump_memory_map(void);


const int HEAPSIZE = (1*1024*1024); // 1 MB
const int MINIMUM_ALLOC = sizeof(int) * 2;

// global file-scope variables for keeping track
// of the beginning of the heap.
void *heap_begin = NULL;

//**********************MALLOC HELPER FUNCTIONS ********************************
//perfect_fit_check , lets you check if the size of the block requested is equal to or greater than the size of the free space your ptr is pointing to. 

int perfect_fit_check(size_t size_block_wanted,uint8_t * ptr_to_block ){
   ptr_to_block =   (uint32_t * )ptr_to_block ;
  size_t size_of_memory_block = (size_t ) ptr_to_block;
  if ((size_of_memory_block /2 ) > size_block_wanted ){
    return 0;
  }else if ((size_of_memory_block /2 ) == size_block_wanted ){
    //allocate
    return 1;
  }else{ printf("WTF"); return 0;}
  
}
//given a size and a pointer to the chunck of free memory, this function allocates that space
void fixed_allocate (size_t size_wanted, uint8_t *ptr ){
  *ptr =(int) size_wanted ;
  ptr ++;
}
//given a ptr to a particular memory block it splits it intwo two and hence changes the free list(this operation is done only on free blocks
void split(uint8_t *ptr){
  if ( (int) *(ptr++) ){//checks if the next of the block is 0 or not. if it isn't then it enters the if statement 
    //----case 1 (the block is the last block of the freelist

    ptr = (uint32_t*) ptr ;
    int size_of_block = (int) *ptr ;
  ptr ++;//pointing to next
  int first_half_next = (int ) *ptr ;
  *ptr = (size_of_block)/2;
  ptr --;//pointing back to size again
  uint8_t * second_half = ptr + (size_of_block)/2;
  
  *second_half =  (size_of_block)/2;
  second_half ++;//pointing to the next of the second half
  *second_half = first_half_next - ((size_of_block)/2);
  }
  else{
   //----case 1 (the block is NOT the last block of the freelist
    ptr = (uint32_t*) ptr ;
  int size_of_block = (int) *ptr ;
  ptr ++;//pointing to next
  int first_half_next = (int ) *ptr ;
  *ptr = (size_of_block)/2;
  ptr --;//pointing back to size again
  uint8_t * second_half = ptr + (size_of_block)/2;
  
  *second_half =  (size_of_block)/2;
  second_half ++;//pointing to the next of the second half
  *second_half = 0;
        
  }

}

//this function cleans up(makes sure our ptr are in the right setup so that free list is correct)
void * post_allocation(void * a_block){
  
}
//this function finds the first block that can provide the size we want
void * find_block(void * free_list, size_t size_wanted){
    struct ret_blocks ;
    uint8_t *free_ptr = (uint8_t *)free_list;
    
    int current_block_size = (int) *((uint32_t *)(free_ptr));
    int current_next  = (int) *(((uint32_t *)(free_ptr))+1);
    while ( current_block_size < size_wanted){
      free_ptr = (uint8_t *) free_ptr ;
      free_ptr += current_next ;//free_ptr pointing to the next block
      
      current_block_size = (int) *((uint32_t *)(free_ptr));
      current_next  = (int) *(((uint32_t *)(free_ptr))+1);
    }

}
		    



//**********************MALLOC HELPER FUNCTIONS ********************************




void *malloc(size_t request_size) {

    // if heap_begin is NULL, then this must be the first
    // time that malloc has been called.  ask for a new
    // heap segment from the OS using mmap and initialize
    // the heap begin pointer.
    if (!heap_begin) {
        heap_begin = mmap(NULL, HEAPSIZE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
        atexit(dump_memory_map);
    }
    void * free_list = heap_begin ;


}

void free(void *memory_block) {

}

void dump_memory_map(void) {

}
int main(){
  return 0;
}
