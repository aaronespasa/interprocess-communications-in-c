#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "servidor.h"

#define INITIAL_CAPACITY 16
#define LOAD_FACTOR 0.75

typedef struct Entry {
    int key;
    Request* value;
    struct Entry *next;
} Entry;

typedef struct HashMap {
    int capacity;
    int size;
    Entry **entries; // [&entry1, &entry2, ...]
} HashMap;

/**
  * This hash function is based on the one proposed by Dan Bernstein (djb2).
  */
unsigned int hash(const char *str) {
  unsigned int hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;  // hash * 33 + c

  return hash;
}

Entry* create_entry(const int key, const Request* value) {
    Entry *entry = malloc(sizeof(Entry));
    entry->key = key;
    entry->value = value;
    entry->next = NULL;
    return entry;
}

HashMap* create_map() {
    HashMap *map = malloc(sizeof(HashMap));
    map->capacity = INITIAL_CAPACITY;
    map->size = 0;
    map->entries = calloc(INITIAL_CAPACITY, sizeof(Entry*));
    return map;
} 

void free_entry(Entry *entry) {
    free(entry->value);
    free(entry);
}

// void free_map(HashMap *map) {
//     for(int i = 0; i < map->capacity; i++) {

//     }
// }