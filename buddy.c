#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>////?????????????????????????
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
//perfect_fit_check , lets you check if the size of the block requested is equal to or greater than the size of the free space your ptr is pointing to. 

int perfect_fit_check(size_t size_block_wanted,uint8_t * ptr_to_block ){
   ptr_to_block =   (uint32_t * )ptr_to_block ;
   int size_of_memory_block = (int) * ptr_to_block;
  if ((size_of_memory_block ) > size_block_wanted ){
    return 0;
  }else if ((size_of_memory_block ) == (size_block_wanted )){
    //allocate
    return 1;
  }else{ printf("WTF"); return 0;}
  
}
//given a size and a pointer to the chunck of free memory, this function allocates that space
void fixed_allocate (size_t size_wanted, uint8_t *ptr ){
  *ptr =(int) size_wanted ;//make sure the size of our block is equal to size_wanted
  ptr ++;//get out pointer to point to "next" of our block
  //  *ptr = 0;//get our blocks "next" to have the value 0 and hence allocated. 
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




  





//**********************MALLOC HELPER FUNCTIONS ********************************




void *the_malloc(size_t request_size) {

    // if heap_begin is NULL, then this must be the first
    // time that malloc has been called.  ask for a new
    // heap segment from the OS using mmap and initialize
    // the heap begin pointer.
    if (!heap_begin) {
        heap_begin = mmap(NULL, HEAPSIZE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
        atexit(dump_memory_map);
    }
    void * free_list = heap_begin ;
    //initalize the free block(aka say we have 1024 bytes and the next is 0

    uint32_t * free_ptr = (uint32_t *)free_list;//free_ptr is pointing to the first int of the free list 
    *free_ptr = 1024;//size
    free_ptr ++;
    *free_ptr = 0;//next 

    //---------Getting the right sized block---------------------
    int size_wanted; // = (int) request_size;

    request_size += 8; //upping the value 
    request_size = (int ) request_size; 
    int power = 0;
    while ( (int) pow(2,power) < (request_size )){//finding the next power of two
      power +=1 ;
    }
    printf("\nthe result : %d\n", (int) pow(2, power));
    size_wanted = (int ) pow(2, power); //this is the size we will actually be allocating
    //---------Getting the right sized block---------------------



    
    int current_block_size = (int) *(free_ptr);
    
    int current_next  = (int) *((free_ptr)+1);//
    int last_next ;
    int old_next ;
    int old_block_size; 
    int trial = 0; 
    while ( current_block_size < (int) size_wanted){
      //      free_ptr = (uint32_t *) free_ptr ;//pointer pointing to the first int
      printf("\ncurren_block_size: %d  size_wanted: %d \n",(int) current_block_size,(int)size_wanted);
      old_block_size = (int) *(free_ptr );//
      old_next = (int) *((free_ptr)+1);//
      free_ptr = (uint8_t *)free_list;//free_ptr is pointing to the first byte of free block
      free_ptr += old_next ;//free_ptr pointing to the next block
      
      free_ptr = (uint32_t *) free_ptr;//free_ptr is pointing to the first int of the next block
      current_block_size = (int) *(free_ptr );// block size
      last_next  = (int) *((free_ptr)+1);//byte offset for next free block
      trial += 1;
    }  

    if (trial ==0){//this means that we found the first free block to be big enough
	     while (perfect_fit_check( ((size_t)size_wanted), ((uint8_t *)free_ptr ))){//checks if the ptr we have is pointing to a memory block equal to the size we want
		split((uint8_t *)free_ptr);//if not we enter the loop and split
	      }
	     //when we fall out of the loop it means that our block's size is exactly 
	      //equal to the size requested so we allocate it 

	     int next_leap_to_free_block = *(free_ptr ++);//get the offset to get to the next free block. this is important because when we allocate 
	  //we will lose the information about how many bytes to go forward to get the next free block.
	      free_list = (void *) free_ptr ;//UPDATE FREE_LIST

	     fixed_allocate( (size_t)size_wanted, (uint8_t *)free_ptr );
    }


    else{//this means if we found the first free block isn't big enough
	  free_ptr = (uint8_t *) free_ptr ;
	  uint8_t * old_ptr = free_ptr ;
	  old_ptr -= last_next ;//This is the a pointer to the old free chunck}

	  while (perfect_fit_check( ((size_t)size_wanted), ((uint8_t *)free_ptr ))){//checks if the ptr we have is pointing to a memory block equal to the size we want
		 //the ptr we have is pointing to a memory block equal to the size we want
		     split((uint8_t *)free_ptr);//if not we enter the loop and split
		   }
		   //when we fall out of the loop it means that our block's size is exactly 
		   //equal to the size requested so we allocate it 
		 free_ptr = (uint32_t *) free_ptr ;//makes sure that free_ptr points to the first int of the perfect fit block to be allocated
		 int next_leap_to_free_block = *(free_ptr ++);//get the offset to get to the next free block. this is important because when we allocate 
	       //we will lose the information about how many bytes to go forward to get the next free block.
		 fixed_allocate( (size_t)size_wanted, (uint8_t *)free_ptr );
		 old_ptr = (uint32_t *)old_ptr;
		 old_ptr ++;
		 *old_ptr += next_leap_to_free_block;//make sure the old_ptr's next is what it needs to be. 
      }

}   
    //-----------------------------------------------FREE LIST--------------------------
/*
void free(void *freeblock) {
  if (freeblock == NULL){
    return; //Do nothing
  }
  else{
    if (freeblock < free_list){//may need to type to uint_32
      uint32_t newoffset = (uint32_t)(free_list - freeblock);
      uint32_t* tmp = (uint32_t *) ((uint32_t *) freeblock + 1);
      * tmp = newoffset; // inputs in type uint8_t for offset
      free_list = freeblock; 
      return;
    }
    else{
      uint32_t * tmp = free_list;
      uint32_t off = (uint32_t) *((uint32_t *) tmp + 1);
      uint32_t * prevptr;
      uint32_t * prevoff;
      while (tmp < (uint32_t *) freeblock){ //iterate until the next block after freeblock 
	prevptr = tmp;
	prevoff = ((uint32_t *) tmp + 1);
	tmp = (uint32_t *) ((uint8_t *) tmp + (int) off);
	off = * ((uint32_t *) tmp + 1);
      }
      uint32_t replaceoff = (uint32_t *) freeblock - prevptr;// offset to new block
      uint32_t freeoff = tmp - (uint32_t) freeblock;// offset to subsequent block
      * prevptr = replaceoff; 
      tmp = (uint32_t *)freeblock + 1;
      * tmp = freeoff; 
      return;
    }
  }
}
*/
void dump_memory_map(void) {
  int place = 0;
  uint32_t * tmp = (uint32_t *) heap_begin;
  while(place < 1024*1024){
    int size = (int) *tmp;
    int offset = (int) *((uint32_t *)tmp + 1);
    char* state;
    if (offset == 0){state = "allocated";}
    else{state = "free";}
    printf("Block size : %d, offset %d, %s", size, offset,state);
    place += size;
    tmp = (uint8_t *) tmp ; 
    tmp  = tmp + offset;
      }
}

int main(){

  char * ptr = (char *) the_malloc(sizeof(char)*20);
  
}


