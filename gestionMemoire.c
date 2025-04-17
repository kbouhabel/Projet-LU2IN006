#include "gestionMemoire.h"

MemoryHandler* memory_init(int size){
    
    Segment* seglist=(Segment*) malloc(sizeof(Segment));
    if (!seglist) return NULL;
    seglist->start=0;
    seglist->size=size;
    seglist->next=NULL;
    
    HashMap* map=hashmap_create();
    if(!map){
        free(seglist);
        return NULL;
    }
    
    void** memory=(void**) malloc(sizeof(void*) * size);
    if(!memory){
        free(seglist);
        hashmap_destroy(map);
        return NULL;
    }
    
    MemoryHandler* MemHand=(MemoryHandler*) malloc(sizeof(MemoryHandler));
    if(!MemHand){
        free(seglist);
        hashmap_destroy(map);
        free(memory);
        return NULL;
    }

    MemHand->memory=memory;
    MemHand->total_size=size;
    MemHand->free_list=seglist;
    MemHand->allocated=map;
    return MemHand;
}

Segment* find_free_segment(MemoryHandler* handler, int start, int size, Segment** prev){
    Segment* temp=NULL;
    Segment* courant=handler->free_list;
    while (courant){
        if ( ((courant->start) <= start) && ((courant->start)+(courant->size)) >= (start+size) ){
            *prev = temp;
            return courant;
        }
        temp=courant;
        courant=courant->next;  
    }
    return NULL;
} 

int create_segment(MemoryHandler* handler, const char* name, int start, int size){
    Segment* prev=NULL;
    Segment* seg=NULL;
    seg=find_free_segment(handler,start,size,&prev);
    if (!seg) return -1;
    if (!prev){ //Si le segment est le premier de la liste 
        
        Segment* seg_end=(Segment*) malloc(sizeof(Segment));
        if (!seg_end) return -1;
        seg_end->start=start + seg->size;
        seg_end->size=seg->size - seg_end->start;
        seg_end->next=seg->next;
        
        handler->free_list=seg_end;
        
        Segment* new_seg=(Segment*) malloc(sizeof(Segment));
        if (!new_seg) return -1;
        new_seg->start=start;
        new_seg->size=size;
        new_seg->next=seg_end;

        
        hashmap_insert(handler->allocated,name,new_seg);
        free(seg);
        return 0;
    }
    else{
        Segment* seg_end=(Segment*) malloc(sizeof(Segment));
        if (!seg_end) return -1;
        seg_end->start=start + seg->size;
        seg_end->size=seg->size - seg_end->start;
        seg_end->next=seg->next; 
        
        Segment* seg_start=(Segment*) malloc(sizeof(Segment));
        seg_start->start=seg->start;
        seg_start->size=start-seg->start;
        seg_start->next=seg_end;

        prev->next=seg_start;
        
        Segment* new_seg=(Segment*) malloc(sizeof(Segment));
        if (!new_seg) return -1;
        new_seg->start=start;
        new_seg->size=size;
        new_seg->next=seg_end;

        
        hashmap_insert(handler->allocated,name,new_seg);
        free(seg);
        return 0;
    }
    return -1;
}

int remove_segment(MemoryHandler* handler, const char* name) {
    Segment* seg = hashmap_get(handler->allocated, name);
    if (!seg) return -1;

    // Remove the segment from the hashmap
    if (hashmap_remove(handler->allocated, name) == -1) return -1;

    Segment* temp = handler->free_list;
    Segment* prev_free = NULL;
    Segment* prev_adjacent = NULL;
    Segment* next_adjacent = NULL;

    // Find adjacent segments
    while (temp) {
        if (temp->start + temp->size == seg->start) {
            prev_adjacent = temp;
        }
        if (seg->start + seg->size == temp->start) {
            next_adjacent = temp;
            break;
        }
        prev_free = temp;
        temp = temp->next;
    }

    // Merge logic
    if (!prev_adjacent && !next_adjacent) {
        temp = handler->free_list;
        prev_free = NULL;
        while (temp && temp->start < seg->start) {
            prev_free = temp;
            temp = temp->next;
        }

        seg->next = temp;
        if (prev_free) {
            prev_free->next = seg;
        } else {
            handler->free_list = seg;
        }
        // CHANGE: Don't free seg here since it's now part of the free list
    } else if (!prev_adjacent && next_adjacent) {
        seg->size += next_adjacent->size;
        seg->next = next_adjacent->next;

        if (prev_free) {
            prev_free->next = seg;
        } else {
            handler->free_list = seg;
        }

        free(next_adjacent);
        // CHANGE: Don't free seg here since it's now part of the free list
    } else if (prev_adjacent && !next_adjacent) {
        prev_adjacent->size += seg->size;
        // CHANGE: Add explicit free here since seg is merged with prev_adjacent
        free(seg);
    } else if (prev_adjacent && next_adjacent) {
        prev_adjacent->size += seg->size + next_adjacent->size;
        prev_adjacent->next = next_adjacent->next;

        free(next_adjacent);
        // CHANGE: Add explicit free here since seg is merged with prev_adjacent
        free(seg);
    }

    // CHANGE: Remove this unconditional free that was causing double-free
    // free(seg);
    // seg = NULL; // Prevent dangling pointer

    return 0;
}