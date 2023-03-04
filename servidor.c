/*
 * servidor.c
 * Authors: 100451339 & 100451170
 */

#include "servidor.h"

/**
 * Insert the element <value1, value2, value3> with key <key>
 */
int list_set_value(int key, char *value1, int value2, double value3, LinkedList* list) {
    // insert request in the linked list
    int error_code = set_value(list, key, value1, value2, value3);
    

    return error_code;
}

int list_get_value(int key, char *value1, int *value2, double *value3, LinkedList *list) {
    // get request from the linked list
    int error_code = get_value(list, key, value1, value2, value3);

    printf("\nLa Key %d existe y se han obtenido sus valores: %s, %d y %f\n", key, value1, *value2, *value3);

    return error_code;
}

int list_modify_value(int key, char *value1, int value2, double value3, LinkedList* list) {
    // create request
    int error_code = modify_value(list, key, value1, value2, value3);
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
