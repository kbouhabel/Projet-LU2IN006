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

int remove_segment(MemoryHandler* handler, const char* name){
    
    Segment* seg=hashmap_get(handler->allocated,name);
    if (!seg) return -1;
    if (hashmap_remove(handler->allocated, name) == -1) {
        return -1; 
    }
    Segment* temp=handler->free_list;
    Segment* prev_seg=NULL;
    Segment* next_seg=NULL;
    while (temp){
        if ((temp->start) == (seg->start)-(temp->size)){
            prev_seg=temp;
        }
        if ((temp->start) == (seg->start)+(seg->size)){
            next_seg=temp;
        }
        temp=temp->next;
    }
    if (!prev_seg && !next_seg){
        Segment* temp=handler->free_list;
        Segment* prec=NULL;
        while (temp){
            
            if (temp->start > seg->start){
                
                if (prec){
                    prec->next=seg;
                }
                
                else{
                    handler->free_list=seg;
                }
                
                seg->next=temp;
                return 0;
            }
            prec=temp;
            temp=temp->next;
        }

        if (prec){
            prec->next=seg;
            seg->next=NULL;
        }
        
        else{
            handler->free_list=seg;
            seg->next=NULL;
        }
        return 0;
    }
    
    if (!prev_seg && next_seg){
        
        seg->size +=  next_seg->size;
        seg->next=next_seg->next;
        free(next_seg);
        return 0;
    }

    if (prev_seg && !next_seg){
        prev_seg->size += seg->size;
        prev_seg->next=NULL;
        free(prev_seg);
        return 0;

    }

    if (prev_seg && next_seg){
        
        prev_seg->size+=seg->size + next_seg->size;
        prev_seg->next=next_seg->next;
        free(next_seg);
        free(seg);
        return 0;
    }
    return -1;
}