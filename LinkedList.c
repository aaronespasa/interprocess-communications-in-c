// gcc -o LinkedList.out LinkedList.c -Wall -Wextra -Werror && ./LinkedList.out
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
  strcpy(entry->value1, value1);
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
    // printf("Key: %d, Value: (%s, %d, %f)\n", current->key, current->value->value1, *(current->value->value2), *(current->value->value3));
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
  * Insert a new entry into the linked list. The entry is inserted sorted by key.
  * Returns 0 if the insertion was successful.
  * Returns -1 if the insertion failed.
  */
int set_value(LinkedList* list, int key, char *value1, int value2, double value3) {
  Entry* entry = create_entry(key, value1, value2, value3);

  if(exist(list, key) == 1) return -1;

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

/**
  * Get the value of an entry with the given key.
  * Return -1 in case the key is not found.
  */
int get_value(LinkedList* list, int key, char *value1, int* value2, double* value3) {
  Entry* entry = search(list, key);
  if (entry == NULL) {
    perror("¡La Key introducida no existe!");
    return -1;
  }
  strcpy(value1, entry->value1);
  *value2 = entry->value2;
  *value3 = entry->value3;
  return 0;
}

/**
  * Modify the value of an entry with the given key.
  * Return -1 in case the key is not found.
  */
int modify_value(LinkedList* list, int key, EntryRequest* value) {
  Entry* entry = search(list, key);
  if (entry == NULL) {
    perror("¡La Key introducida no existe!");
    return -1;
  }
  entry->value = value;
  return 0;
}

/**
  * Delete an entry with the given key.
  * Return -1 in case the key is not found.
  */
int delete_key(LinkedList* list, int key) {
  Entry* entry = search(list, key);
  if (entry == NULL) {
    perror("¡La Key introducida no existe!");
    return -1;
  }

  // If the entry is the head of the list, delete it.
  if (entry == list->head) {
    list->head = entry->next;
    delete_entry(entry);
    list->size--;
    return 0;
  }

  // If the entry is not the head of the list, delete it.
  Entry* previous = list->head;
  Entry* current = list->head->next;
  while(current != NULL) {
    if (current == entry) {
      previous->next = current->next;
      delete_entry(current);
      list->size--;
      return 0;
    }
    previous = current;
    current = current->next;
  }

  return -1;
}

/**
  * Copy the value of an entry with the given key.
  * Return -1 in case the key is not found.
  */
 int copy_key(LinkedList* list, int key1, int key2) {
  Entry* entry1 = search(list, key1);
  if (entry1 == NULL) {
    perror("¡La Key que se quiere copiar no existe!");
    return -1;
  }

  // If key2 exists, modify its value.
  // If key2 does not exist, create a new entry with key2 and the value of key1.
  Entry* entry2 = search(list, key2);
  if (entry2 == NULL) {
    set_value(list, key2, entry1->value);
  } else {
    entry2->value = entry1->value;
  }
  
  return 0;
 }

// int main() {
//   LinkedList* list = create_linked_list();
//   set_value(list, 4, NULL);
//   set_value(list, 2, NULL);
//   set_value(list, 3, NULL);
//   delete_key(list, 4);
//   display_list(list);
//   delete_linked_list(list);
// }