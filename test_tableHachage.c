#include "tableHachage.h"
#include <assert.h>

void test_hashmap() {
    // Create a new hash map
    HashMap* map = hashmap_create();
    assert(map != NULL);

    // Test insertion
    assert(hashmap_insert(map, "key1", "value1") == 0);
    assert(hashmap_insert(map, "key2", "value2") == 0);

    // Test retrieval
    assert(strcmp((char*)hashmap_get(map, "key1"), "value1") == 0);
    assert(strcmp((char*)hashmap_get(map, "key2"), "value2") == 0);
    assert(hashmap_get(map, "key3") == NULL);

    // Test removal
    assert(hashmap_remove(map, "key1") == 0);
    assert(hashmap_get(map, "key1") == NULL);

    // Test tombstone handling
    assert(hashmap_insert(map, "key1", "new_value1") == 0);
    assert(strcmp((char*)hashmap_get(map, "key1"), "new_value1") == 0);

    // Test destruction
    hashmap_destroy(map);
    printf("All tests passed successfully.\n");
}

int main() {
    test_hashmap();
    return 0;
}
