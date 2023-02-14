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

Entry* CreateEntry(int key, Request* value) {
  Entry *entry = (Entry *)malloc(sizeof(Entry)); // Cast to Entry pointer type
  entry->key = key;
  entry->value = value;
  entry->next = NULL;
  return entry;
}

void DeleteEntry(Entry* entry) {
  free(entry);
}

void DeleteLinkedList(LinkedList* list) {
  Entry* current = list->head;
  while(current != NULL) {
    Entry* next = current->next;
    DeleteEntry(current);
    current = next;
  }
  free(list);
}

LinkedList* CreateLinkedList() {
  LinkedList* list = (LinkedList *)malloc(sizeof(LinkedList));
  list->head = NULL;
  list->size = 0;
  return list;
}

void displayList(LinkedList* list) {
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

Entry* GetNextNode(Entry* entry) {
  return entry->next;
}

int insert(LinkedList* list, int key, Request* value) {
  Entry* entry = CreateEntry(key, value);

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
  LinkedList* list = CreateLinkedList();
  insert(list, 4, NULL);
  insert(list, 2, NULL);
  insert(list, 3, NULL);
  displayList(list);
  DeleteLinkedList(list);
}