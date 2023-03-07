#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct Entry
{
    int key;
    char value1[256];
    int value2;
    double value3;
    struct Entry *next;
} Entry;

// Implement a linked list of Entry
typedef struct
{
    Entry *head;
    int size;
} LinkedList;

/**
 * Search for an entry with the given key.
 * Returns NULL if the key is not found.
 */
Entry *search(LinkedList *list, int key);

/**
 * Create a new entry with the given key and value.
 */
Entry *create_entry(int key, char *value1, int value2, double value3);

/**
 * Delete an entry from the linked list.
 */
void delete_entry(Entry *entry);

/**
 * Delete the entire linked list.
 */
void delete_linked_list(LinkedList *list);

/**
 * Insert a new entry into the list.
 * If an entry with the same key already exists, it is replaced.
 */
LinkedList *create_linked_list();

/**
 * Insert a new entry into the list.
 * If an entry with the same key already exists, it is replaced.
 */
void display_list(LinkedList *list);

/**
 * Check if an entry with the given key exists.
 * Return 1 if the key exists.
 * Return 0 if the key does not exist.
 */
int exist(LinkedList *list, int key);

/**
 * @brief Initialise service and destroys all stored tuples.
 * @return 0 if the service was initialised correctly, -1 an error occurred during communication.
 */
int init(LinkedList *list);

/**
 * @brief Insert a new element clave-valor1-valor2-valor3 in the service.
 * @param key int
 * @param value1 char*
 * @param value2 int
 * @param value3 double
 * @return 0 if the insertion was successful; -1 if the insertion failed.
 */
int set_value(LinkedList *list, int key, char *value1, int value2, double value3);

/**
 * @brief Assign the values of the element associated to the key to the variables value1, value2 and value3.
 * @param key int
 * @param value1 char*
 * @param value2 int
 * @param value3 double
 * @return 0 if the element exists and the values were assigned correctly; -1 if the element does not exist, or an error occurred during communication.
 * @note Values are assigned to the variables passed as parameters.
 */
int get_value(LinkedList *list, int key, char *value1, int *value2, double *value3);

/**
 * @brief
 * @param key
 * @param value1 char*
 * @param value2 int
 * @param value3 double
 * @return 0 if the element exists and the values were modified correctly; -1 if the element does not exist, or an error occurred during communication.
 */
int modify_value(LinkedList *list, int key, char *value1, int value2, double value3);

/**
 * @brief Delete the element associated to the key.
 * @param key int
 * @return 0 if success; -1 if the element does not exist, or an error occurred during communication.
 */
int delete_key(LinkedList *list, int key);

/**
 * @brief Copy the element associated to the key1 to the key2.
 * @param key1 int
 * @param key2 int
 * @return 0 if values were copied correctly to key2; -1 if the key1 does not exist, or an error occurred during communication.
 */
int copy_key(LinkedList *list, int key1, int key2);

#endif
