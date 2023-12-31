/*
 * File: LinkedList.c
 * Authors: 100451339 & 100451170
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "LinkedList.h"

/**
 * Search for an entry with the given key.
 * Returns NULL if the key is not found.
 */
Entry *search(LinkedList *list, int key)
{
  Entry *current = list->head;
  while (current != NULL)
  {
    if (current->key == key)
    {
      return current;
    }
    current = current->next;
  }
  return NULL;
}

/**
 * Create a new entry with the given key and value.
 */
Entry *create_entry(int key, char *value1, int value2, double value3)
{
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
void delete_entry(Entry *entry)
{
  free(entry);
}

void delete_list(LinkedList *list)
{
  free(list);
}

/**
 * Delete the entire linked list.
 */
void delete_linked_list(LinkedList *list)
{
  Entry *current = list->head;
  while (current != NULL)
  {
    Entry *next = current->next;
    delete_entry(current);
    current = next;
  }
  delete_list(list);
}

/**
 * Create a new empty linked list.
 */
LinkedList *create_linked_list()
{
  LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
  list->head = NULL;
  list->size = 0;
  return list;
}

/**
 * Display the linked list.
 */
void display_list(LinkedList *list)
{
  printf("\n------------ Linked List ------------\n");

  int iter = 0;
  Entry *current = list->head;

  while (current != NULL)
  {
    printf("\nComponents of the entry %d (%p): [", iter, current);
    printf("key: %d, ", current->key);
    printf("val1: %s, ", current->value1);
    printf("val2: %d, ", current->value2);
    printf("val3: %f]\n", current->value3);
    current = current->next;
    iter++;
  }
  printf("\nTotal size: %d\n", list->size);
  printf("-------------------------------------\n");
}

/**
 * Check if an entry with the given key exists.
 * Return 1 if the key exists.
 * Return 0 if the key does not exist.
 */
int exist(LinkedList *list, int key)
{
  Entry *entry = search(list, key);
  if (entry == NULL)
  {
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

int init(LinkedList *list)
{
  printf("\nInitializing the linked list...\n");
  Entry *current = list->head;
  while (current != NULL)
  {
    Entry *next = current->next;
    delete_entry(current);
    current = next;
  }
  list->head = NULL; // Update the head of the list to NULL as it is empty now
  list->size = 0;    // Update the size of the list
  return 0;
}

/**
 * Insert a new entry into the linked list. The entry is inserted sorted by key.
 * Returns 0 if the insertion was successful.
 * Returns -1 if the insertion failed.
 */

int set_value(LinkedList *list, int key, char *value1, int value2, double value3)
{
  if (exist(list, key) == 1)
  {
    perror("\nThe provided key already exists");
    return -1;
  }

  Entry *entry = create_entry(key, value1, value2, value3);

  // If the list is empty, insert the entry at the head.
  if (list->head == NULL)
  {
    list->head = entry;
    list->size++;
    return 0;
  }

  // If the list is not empty, insert the entry in the correct position (sorted by key).
  Entry *previous = NULL;
  Entry *current = list->head;
  // While the current entry is not NULL and the key of the current entry is less than the key of the new entry
  while (current != NULL && current->key < key)
  {
    previous = current;
    current = current->next;
  }
  // If the previous entry is NULL, means that the current entry is the head of the list.
  if (previous == NULL)
  {
    entry->next = current;
    list->head = entry;
  }
  // If the previous entry is not NULL, means that the current entry is not the head of the list.
  else
  {
    entry->next = current;
    previous->next = entry;
  }
  list->size++;
  return 0;
}

/**
 * Get the value of an entry with the given key.
 * Return -1 in case the key is not found.
 */
int get_value(LinkedList *list, int key, char *value1, int *value2, double *value3)
{
  Entry *entry = search(list, key);

  if (entry == NULL)
  {
    perror("\nThe provided key does not exist");
    return -1;
  }

  // Comprobamos que los punteros de value1, value2 y value3 no son NULL
  if (value1 == NULL || value2 == NULL || value3 == NULL)
  {
    perror("\nOne or more of the value pointers is NULL!");
    return -1;
  }

  // Si la key existe, copiamos los valores en los punteros de value1, value2 y value3

  strcpy(value1, entry->value1);
  *value2 = entry->value2;
  *value3 = entry->value3;

  return 0;
}

/**
 * Modify the value of an entry with the given key.
 * Return -1 in case the key is not found.
 */
int modify_value(LinkedList *list, int key, char *value1, int value2, double value3)
{
  Entry *entry = search(list, key);
  if (entry == NULL)
  {
    perror("\nThe provided key does not exist");
    return -1;
  }
  strcpy(entry->value1, value1);
  entry->value2 = value2;
  entry->value3 = value3;
  return 0;
}

/**
 * Delete an entry with the given key.
 * Return -1 in case the key is not found.
 */
int delete_key(LinkedList *list, int key)
{
  Entry *entry = search(list, key);
  if (entry == NULL)
  {
    perror("\nThe provided key does not exist");
    return -1;
  }
  // If the entry is the head of the list, delete it.
  if (entry == list->head)
  {
    list->head = entry->next;
    delete_entry(entry);
    list->size--;
    return 0;
  }
  // If the entry is not the head of the list, delete it.
  Entry *previous = list->head;
  Entry *current = list->head->next;
  while (current != NULL)
  {
    if (current == entry)
    {
      previous->next = current->next;
      delete_entry(current);
      list->size--;
      return 0;
    }
    previous = current;
    current = current->next;
  }
  // If the entry is not found, return -1.
  return -1;
}

// /**
//   * Copy the value of an entry with the given key.
//   * Return -1 in case the key is not found.
//   */
int copy_key(LinkedList *list, int key1, int key2)
{
  Entry *entry1 = search(list, key1);
  if (entry1 == NULL)
  {
    perror("\nThe provided key to copy to already exists");
    return -1;
  }

  // If key2 exists, modify its value.
  // If key2 does not exist, create a new entry with key2 and the value of key1.
  Entry *entry2 = search(list, key2);
  if (entry2 == NULL)
  {
    set_value(list, key2, entry1->value1, entry1->value2, entry1->value3);
  }
  else
  {
    modify_value(list, key2, entry1->value1, entry1->value2, entry1->value3);
  }

  return 0;
}

// int main(void)
// {
//   LinkedList *list = create_linked_list();
//   set_value(list, 1, "Hola", 1, 1.0);
//   set_value(list, 2, "Hola", 2, 2.0);
//   set_value(list, 5, "Hola", 3, 3.0);
//   set_value(list, 6, "No", 4, 9.0);
//   set_value(list, 7, "Hola", 3, 3.0);
//   set_value(list, 1, "Hola", 4, 4.0);
//   set_value(list, 3, "Hola", 5, 5.0);
//   modify_value(list, 1, "Hola", 6, 6.0);
//   display_list(list);
//   delete_key(list, 1);
//   delete_key(list, 7);
//   display_list(list);
//   int e0 = exist(list, 1);
//   int e1 = exist(list, 2);
//   printf("¿Existe la key 1? %d, ¿Existe la key 2? %d", e0, e1);
//   copy_key(list, 2, 1);
//   copy_key(list, 5, 6);
//   display_list(list);
//   delete_linked_list(list);

//   test_get_value();
// }

// void test_get_value() {
//   // Create a new empty linked list
//   LinkedList *list = create_linked_list();

//   // Add some entries to the linked list
//   set_value(list, 1, "value1", 2, 3.14);
//   set_value(list, 2, "value2", 4, 6.28);
//   set_value(list, 3, "value3", 6, 9.42);

//   // Test getting an existing value
//   char value1[256];
//   int value2;
//   double value3;
//   int result = get_value(list, 2, value1, &value2, &value3);
//   if (result == 0) {
//     printf("OKAY: got value: key=2, value1=%s, value2=%d, value3=%f\n", value1, value2, value3);
//   } else {
//     printf("ERROR: Error getting value for key=2\n");
//   }

//   // Test getting a non-existent value
//   result = get_value(list, 4, value1, &value2, &value3);
//   if (result == 0) {
//     printf("ERROR: got value for non-existent key\n");
//   } else {
//     printf("OKAY: not getting non-existent value\n");
//   }

//   // Free the linked list
//   delete_linked_list(list);
// }