#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#include "LinkedList.h"

#define TRUE 1
#define FALSE 0

/**
 * @brief Initialise service and destroys all stored tuples.
 * @param list LinkedList
 * @return 0 if the service was initialised correctly, -1 an error occurred during communication.
 * @note This is a WRITER function.
 */
int list_init(LinkedList *list);

/**
 * @brief Insert a new element clave-valor1-valor2-valor3 in the service.
 * @param key int
 * @param value1 char*
 * @param value2 int
 * @param value3 double
 * @return 0 if the insertion was successful; -1 if the insertion failed.
 * @note This is a WRITER function.
 */
int list_set_value(int key, char *value1, int value2, double value3, LinkedList *list);

/**
 * @brief Assign the values of the element associated to the key to the variables value1, value2 and value3.
 * @param key int
 * @param value1 char*
 * @param value2 int
 * @param value3 double
 * @return 0 if the element exists and the values were assigned correctly; -1 if the element does not exist, or an error occurred during communication.
 * @note This is a READER function.
 */
int list_get_value(int key, char *value1, int *value2, double *value3, LinkedList *list);

/**
 * @brief Modify the values of the element associated to the key.
 * @param key
 * @param value1 char*
 * @param value2 int
 * @param value3 double
 * @return 0 if the element exists and the values were modified correctly; -1 if the element does not exist, or an error occurred during communication.
 * @note This is a WRITER function.
 */
int list_modify_value(int key, char *value1, int value2, double value3, LinkedList *list);

/**
 * @brief Delete the element associated to the key.
 * @param key int
 * @return 0 if success; -1 if the element does not exist, or an error occurred during communication.
 * @note This is a WRITER function.
 */
int list_delete_key(int key, LinkedList *list);

/**
 * @brief Check if the element associated to the key exists.
 * @param key int
 * @return 1 if exists; 0 if not exists; -1 if error occurred during communication.
 * @note This is a READER function.
 */
int list_exist(int key, LinkedList *list);

/**
 * @brief Copy the element associated to the key1 to the key2.
 * @param key1 int
 * @param key2 int
 * @return 0 if values were copied correctly to key2; -1 if the key1 does not exist, or an error occurred during communication.
 * @note This is a WRITER function.
 */
int list_copy_key(int key1, int key2, LinkedList *list);

/**
 * @brief Display the list.
 * @param list LinkedList
 * @note This is a READER function.
 */
void list_display_list(LinkedList *list);

#endif