#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#include "LinkedList.c"

#define TRUE 1
#define FALSE 0

/**
 * Insert the element <value1, value2, value3> with key <key>
 */
int list_set_value(int key, char *value1, int value2, double value3, LinkedList* list);

int list_get_value(int key, char *value1, int *value2, double *value3, LinkedList *list);

int list_modify_value(int key, char *value1, int value2, double value3, LinkedList *list);

int list_delete_key(int key, LinkedList *list);

int list_exist(int key, LinkedList *list);

int list_copy_key(int key1, int key2, LinkedList *list);

void list_display_list(LinkedList *list);

#endif