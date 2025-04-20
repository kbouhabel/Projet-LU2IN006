#include "tableHachage.h"


unsigned long simple_hash(const char* str){
    unsigned long hash = 0;
    for (; *str; str++){
        hash = ( *str + 23 * hash) % TABLE_SIZE;
    }
    return hash;
}

HashMap* hashmap_create(){
    HashMap* map = (HashMap*) malloc(sizeof(HashMap));
    if (!map) return NULL;

    map->size = TABLE_SIZE;
    map->table = (HashEntry*) malloc(sizeof(HashEntry) * TABLE_SIZE);
    if(!(map->table)){
        free(map);
        return NULL;
    } 

    for (int i =0; i<TABLE_SIZE; i++)  {
        map->table[i].key = NULL;
        map->table[i].value = NULL;
    }
    return map;
}

int hashmap_insert(HashMap* map, const char* key, void* value) {
    unsigned long index = simple_hash(key);
    for (unsigned long i = 0; i < TABLE_SIZE; i++) {
        unsigned long new_index = (index + i) % TABLE_SIZE;

        if (map->table[new_index].key == NULL || map->table[new_index].key == TOMBSTONE) {
            map->table[new_index].key = strdup(key); 
            map->table[new_index].value = value;
            printf("Inserted key: %s, value: %p\n", key, value);
            return 0;
        }
    }
    return -1;
}

void* hashmap_get(HashMap* map, const char* key){
    unsigned long index= simple_hash(key);
    for (unsigned long i=0; i<TABLE_SIZE;i++){
        unsigned long new_index = (index + i) % TABLE_SIZE;
        if (map->table[new_index].key != NULL && map->table[new_index].key != TOMBSTONE && strcmp(map->table[new_index].key,key)==0){
            return map->table[new_index].value;
        }
    }
    return NULL;
}

int hashmap_remove(HashMap* map, const char* key) {
    unsigned long index = simple_hash(key);
    for (unsigned long i = 0; i < TABLE_SIZE; i++) {
        unsigned long new_index = (index + i) % TABLE_SIZE;
        if (map->table[new_index].key == NULL) {
            continue;
        }

        if (map->table[new_index].key == TOMBSTONE) {
            continue;
        }

        if (strcmp(map->table[new_index].key, key) == 0) {
            free(map->table[new_index].key);
            map->table[new_index].key = TOMBSTONE;

            // Do not free the value here
            map->table[new_index].value = TOMBSTONE;
            return 0;
        }
    }
    return -1;
}

void hashmap_destroy(HashMap* map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (map->table[i].key && map->table[i].key != TOMBSTONE) {
            printf("Freeing key: %s\n", map->table[i].key);
            free(map->table[i].key);
        }
        if (map->table[i].value && map->table[i].value != TOMBSTONE) {
            printf("Freeing value: %p\n", map->table[i].value);
            free(map->table[i].value);
        }
    }
    free(map->table);
    free(map);
}