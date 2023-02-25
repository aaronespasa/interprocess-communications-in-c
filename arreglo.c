#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

typedef struct Entry {
  int key;
  char value1[256];
  int value2;
  double value3;
  struct Entry *next;
} Entry;

// Implement a linked list of Entry
typedef struct {
  Entry *head;
  int size;
} LinkedList;

/**
  * Search for an entry with the given key.
  * Returns NULL if the key is not found.
  */
Entry* search(LinkedList *list, int key) {
  Entry* current = list->head;
  while(current != NULL) {
    if(current->key == key) {
      return current;
    }
    current = current->next;
  }
  return NULL;
}

/**
  * Create a new entry with the given key and value.
  */
Entry* create_entry(int key, char *value1, int value2, double value3) {
  Entry *entry = (Entry *)malloc(sizeof(Entry)); // Cast to Entry pointer type
  entry->key = key;
  strncpy(entry->value1, value1, sizeof(entry->value1)-1); // Use strncpy instead of strcpy to avoid buffer overflow
  entry->value2 = value2;
  entry->value3 = value3;
  entry->next = NULL;
  return entry;
}

/**
  * Delete an entry from the linked list.
  */
void delete_entry(Entry* entry) {
  free(entry);
}

/**
  * Delete the entire linked list.
  */
void delete_linked_list(LinkedList* list) {
  Entry* current = list->head;
  while(current != NULL) {
    Entry* next = current->next;
    delete_entry(current);
    current = next;
  }
  free(list);
}

/**
  * Create a new empty linked list.
  */
LinkedList* create_linked_list() {
  LinkedList* list = (LinkedList *)malloc(sizeof(LinkedList));
  list->head = NULL;
  list->size = 0;
  return list;
}

/**
  * Display the linked list.
  */
void display_list(LinkedList* list) {
  printf("------------ Linked List ------------\n");
  Entry* current = list->head;
  while(current != NULL) {
    if(current->next == NULL) {
      printf("%d\n", current->key);
    } else {
      printf("%d -> ", current->key);
    }
    current = current->next;
  }
  printf("\nKeys of the linked list:\n");

  current = list->head;
  while(current != NULL) {
    printf("Key: %d\n", current->key);
    printf("Value1: %s\n", current->value1);
    printf("Value2: %d\n", current->value2);
    printf("Value3: %f\n", current->value3);
    current = current->next;
  }
  printf("-------------------------------------\n");
}

/**
  * Check if an entry with the given key exists.
  * Return 1 if the key exists.
  * Return 0 if the key does not exist.
  */
int exist(LinkedList* list, int key) {
  Entry* entry = search(list, key);
  if (entry == NULL) {
    return 0;
  }
  return 1;
}

/* 
 * Program specfic functions:
 * - set_value
 * - get_value
 * - modify_value
 * - delete_key
 * - exist (defined above)
 * - copy_key
 */

/**
  * Insert a new entry into the
