#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gestionMemoire.h"

void test_memory_init() {
    MemoryHandler* handler = memory_init(100);
    if (handler && handler->total_size == 100 && handler->free_list && handler->allocated) {
        printf("test_memory_init: PASSED\n");
    } else {
        printf("test_memory_init: FAILED\n");
    }
    // Cleanup
    free(handler->memory);
    Segment* temp;
    while (handler->free_list) {
        temp = handler->free_list;
        handler->free_list = handler->free_list->next;
        free(temp);
    }
    hashmap_destroy(handler->allocated);
    free(handler);
}

void test_create_segment() {
    MemoryHandler* handler = memory_init(100);
    if (!handler) {
        printf("test_create_segment: FAILED (Initialization Error)\n");
        return;
    }

    if (create_segment(handler, "segment1", 0, 50) == 0) {
        printf("test_create_segment: PASSED\n");
    } else {
        printf("test_create_segment: FAILED\n");
    }

    // Cleanup
    free(handler->memory);
    Segment* temp;
    while (handler->free_list) {
        temp = handler->free_list;
        handler->free_list = handler->free_list->next;
        free(temp);
    }
    hashmap_destroy(handler->allocated);
    free(handler);
}

void test_remove_segment() {
    MemoryHandler* handler = memory_init(100);
    if (!handler) {
        printf("test_remove_segment: FAILED (Initialization Error)\n");
        return;
    }

    create_segment(handler, "segment1", 0, 50);

    if (remove_segment(handler, "segment1") == 0) {
        printf("test_remove_segment: PASSED\n");
    } else {
        printf("test_remove_segment: FAILED\n");
    }

    // Cleanup
    Segment* temp;
    while (handler->free_list) {
        temp = handler->free_list;
        handler->free_list = handler->free_list->next;
        free(temp);
    }
    hashmap_destroy(handler->allocated);
    free(handler->memory);
    free(handler);
}

int main() {
    test_memory_init();
    test_create_segment();
    test_remove_segment();
    return 0;
}
