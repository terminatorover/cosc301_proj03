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
int perfect_fit_check(size_t size_block_wanted,unit8_t * ptr_to_block ){
  unit32_t * ptr_to_block =   (unit32_t * )ptr_to_block ;
  size_t size_of_memory_block = (size_t ) ptr_to_block;
  if ((size_of_memory_block /2 ) > size_block_wanted ){
    return 0;
  }else if ((size_of_memory_block /2 ) == size_block_wanted ){
    //allocate
    return 1;
  }else{ printf("WTF"); return 0;}
  
}

void allocate (





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

