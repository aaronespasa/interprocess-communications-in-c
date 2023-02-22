/*
 * servidor.c
 * Authors: 100451339 & 100451170
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.c"

/**
 * Insert the element <value1, value2, value3> with key <key>
 */
int list_set_value(int key, char *value1, int value2, double value3, LinkedList* list) {
    // create request
    EntryRequest* request = (EntryRequest *)malloc(sizeof(EntryRequest));
    strcpy(request->value1, value1);
    request->value2 = &value2;
    request->value3 = &value3;

    // insert request in the linked list
    int error_code = set_value(list, key, request);

    return error_code;
}

int list_get_value(int key, char *value1, int* value2, double* value3, LinkedList* list) {
    // create request
    EntryRequest* request = (EntryRequest *)malloc(sizeof(EntryRequest));
    strcpy(request->value1, value1);
    request->value2 = value2;
    request->value3 = value3;

    // get request from the linked list
    int error_code = get_value(list, key, request);

    return error_code;
}

int list_modify_value(int key, char *value1, int value2, double value3, LinkedList* list) {
    // create request
    EntryRequest* request = (EntryRequest *)malloc(sizeof(EntryRequest));
    strcpy(request->value1, value1);
    request->value2 = &value2;
    request->value3 = &value3;

    // modify request in the linked list
    int error_code = modify_value(list, key, request);

    return error_code;
}

int list_delete_key(int key, LinkedList* list) {
    // delete request from the linked list
    int error_code = delete_key(list, key);

    return error_code;
}

int list_exist(int key, LinkedList* list) {
    // check if key exists in the linked list
    int error_code = exist(list, key);

    return error_code;
}

int list_copy_key(int key1, int key2, LinkedList* list) {
    // copy request from key1 to key2 in the linked list
    int error_code = copy_key(list, key1, key2);

    return error_code;
}

void list_display_list(LinkedList* list) {
    display_list(list);
}