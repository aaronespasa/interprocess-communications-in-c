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

Entry* create_entry(int key, Request* value) {
  Entry *entry = (Entry *)malloc(sizeof(Entry)); // Cast to Entry pointer type
  entry->key = key;
  entry->value = value;
  entry->next = NULL;
  return entry;
}

void delete_entry(Entry* entry) {
  free(entry);
}

void delete_linked_list(LinkedList* list) {
  Entry* current = list->head;
  while(current != NULL) {
    Entry* next = current->next;
    delete_entry(current);
    current = next;
  }
  free(list);
}

LinkedList* create_linked_list() {
  LinkedList* list = (LinkedList *)malloc(sizeof(LinkedList));
  list->head = NULL;
  list->size = 0;
  return list;
}

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
int set_value(LinkedList* list, int key, Request* value) {
  Entry* entry = create_entry(key, value);

  if (list->head == NULL) {
    list->head = entry;
    list->size++;
    return 0;
  }

  Entry* previous = NULL;
  Entry* current = list->head;
  while(current != NULL) {
    if (current->key == key) {
      perror("¡La Key introducida ya existe!");
      return -1;
    }

    if (current->key > key) {
      if (previous == NULL) {
        entry->next = current;
        list->head = entry;
      } else {
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