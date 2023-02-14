// gcc -o LinkedList.out LinkedList.c -Wall -Wextra -Werror && ./LinkedList.out
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "servidor.h"

typedef struct Entry {
  int key;
  Request* value;
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
Entry* create_entry(int key, Request* value) {
  Entry *entry = (Entry *)malloc(sizeof(Entry)); // Cast to Entry pointer type
  entry->key = key;
  entry->value = value;
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
  Entry* current = list->head;
  while(current != NULL) {
    if(current->next == NULL) {
      printf("%d\n", current->key);
    } else {
      printf("%d -> ", current->key);
    }
    current = current->next;
  }
}

/* 
 * Program specfic functions:
 * - set_value
 * - get_value
 * - modify_value
 * - delete_key
 * - exist
 * - copy_key
 */

/**
  * Insert a new entry into the linked list. The entry is inserted sorted by key.
  * Returns 0 if the insertion was successful.
  * Returns -1 if the insertion failed.
  */
int set_value(LinkedList* list, int key, Request* value) {
  Entry* entry = create_entry(key, value);

  // If the list is empty, insert the entry at the head.
  if (list->head == NULL) {
    list->head = entry;
    list->size++;
    return 0;
  }

  // If the list is not empty, insert the entry in the correct position (sorted by key).
  Entry* previous = NULL;
  Entry* current = list->head;
  while(current != NULL) {
    // In case the current entry has the same key as the key of the new entry,
    // an error must be thrown indicating that the key already exists.
    if (current->key == key) {
      perror("¡La Key introducida ya existe!");
      return -1;
    }

    // In case the current entry has a key greater than the key of the new entry,
    // this means that the new entry should be inserted just before the current entry.
    if (current->key > key) {
      if (previous == NULL) {
        // If the previous entry is NULL,
        // this means that the current entry is the head of the list.
        entry->next = current;
        list->head = entry;
      } else {
        // If the previous entry is not NULL,
        // this means that the current entry is not the head of the list.
        entry->next = current;
        previous->next = entry;
      }
      list->size++;
      return 0;
    }

    previous = current;
    current = current->next;
  } 

  return -1;
}



int main() {
  LinkedList* list = create_linked_list();
  set_value(list, 4, NULL);
  set_value(list, 2, NULL);
  set_value(list, 3, NULL);
  display_list(list);
  delete_linked_list(list);
}