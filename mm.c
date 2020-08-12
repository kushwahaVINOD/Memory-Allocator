
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"
//memory block structure
struct Node{
    struct Node* next;
    char isfree;
    size_t size;
    };

//function definations
struct Node* findFreeFF(size_t fsize);
void addFreeNode(struct Node* node1);
void mmoptimize();


team_t team = {
    /* Team name */
    "Lions",
    /* First member's full name */
    "Vinod Kushwaha",
    /* First member's email address */
    "193050050@iitb.ac.in",
    /* Second member's full name (leave blank if none) */
    "Billoh Gassama",
    /* Second member's email address (leave blank if none) */
    "193051001@iitb.ac.in"
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

char *mmStart, *nodeEnd;
//int c=1;

char* nodestart;
static size_t sizeofstruct=ALIGN((size_t)sizeof(struct Node));

int mm_init(void)
{
    nodestart=NULL;
    nodeEnd=NULL;

    return 0;
}


void *mm_malloc(size_t size)
{
 
    size_t newsize=0;
    size = ALIGN(size + SIZE_T_SIZE);
   
    struct Node* mblock = findFreeFF(size);
    if(mblock==NULL){
        newsize = size +sizeofstruct;
        mblock=(struct Node*)mem_sbrk(newsize);
       
        if ((void *)mblock == (void *)-1)
            return NULL;
        else {
            mblock->size=size;
            mblock->isfree='f';
            mblock->next=NULL;
            addFreeNode(mblock);
                  
            char *p =((char *)mblock + sizeofstruct);
         
            return (void *)(p + SIZE_T_SIZE);
        }
    }  
    else{
        char *p =((char *)mblock + sizeofstruct);
          
            return (void *)(p + SIZE_T_SIZE);

    } 
}

void mm_free(void *ptr12)
{
    char *ptr1=((char *)ptr12-SIZE_T_SIZE);
    struct Node* ptr=(struct Node*)(ptr1 - sizeofstruct);
   
    ptr->isfree='t';
   
    mmoptimize(); 
    
}


void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    if(size==0)
       { mm_free(ptr);
        return NULL;}
    newptr = mm_malloc(size);
    
    if (newptr == NULL)
      return NULL;
    
    ptr=(((char *)oldptr - SIZE_T_SIZE) - sizeofstruct);
    struct Node* temp=(struct Node*)ptr;
    copySize = temp->size;
    
    if (size < copySize)
      copySize = size;
   
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}


struct Node* findFreeFF(size_t fsize){
    
    
        struct Node* lastfree =(struct Node*) nodestart;
        while(lastfree!=NULL ){
            if(lastfree->isfree=='t' && lastfree->size >=(fsize))
                {break;}
            lastfree=lastfree->next;
        }
        if(lastfree==NULL)
            return NULL;
        else{
            if(lastfree->size >(sizeofstruct + fsize + (size_t)10)){
               
                struct Node* newnode=(struct Node*)((char *)lastfree+(sizeofstruct+fsize));
               
                newnode->size=(lastfree->size)-fsize-sizeofstruct;
                newnode->isfree='t';
                newnode->next=lastfree->next;
                lastfree->next=newnode;
                lastfree->size=fsize;
        }
            lastfree->isfree='f';
            return lastfree;    
        }
       
    

}





//appending to linked list
void addFreeNode(struct Node* node1){

     struct Node* lastfree = (struct Node*)nodeEnd;
     if(nodestart==NULL){
        nodestart=(char *)node1;
        nodeEnd=nodestart;
        

     }
     else{
       
        lastfree->next=node1;
        nodeEnd=(char *)node1;
    }
}


//merging free nodes
void mmoptimize(){
    struct Node* lastfree=NULL;
    lastfree = (struct Node*)nodestart;
    
    while(lastfree!=NULL && lastfree->next!=NULL){
        if (lastfree->isfree=='t' && (lastfree->next)->isfree=='t')
        {
          lastfree->size+=sizeofstruct+(lastfree->next)->size;
          lastfree->next=(lastfree->next)->next;
          if(lastfree->next==NULL)
            nodeEnd=(char *)lastfree;
          break;
        }
        lastfree=lastfree->next; 
       
    }
}









