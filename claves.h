#ifndef CLAVES_H
#define CLAVES_H

#include <fcntl.h>    /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <sys/types.h> /* For mode constants */
#include <sys/socket.h> /* For socket, bind, listen, accept */
#include <netinet/in.h> /* For sockaddr_in */
#include <arpa/inet.h>
#include <string.h> /* For strlen, strcpy, sprintf */
#include <unistd.h> /* For sleep */
#include <pthread.h> /* For threads */

#include <stdio.h>  /* For printf */
#include <stdlib.h> /* For exit */

#include "request.h"  /* For request struct */
#include "lines.h" /* For line functions */

#define NUM_THREADS 3 /* Number of threads */

#define MQ_SERVER "/mq_server" /* Queue name */

#define true 1
#define false 0

/**
 * @brief Initialise service and destroys all stored tuples.
 * @return 0 if the service was initialised correctly, -1 an error occurred during communication.
 */
int init();

/**
 * @brief Insert a new element clave-valor1-valor2-valor3 in the service.
 * @param key int
 * @param value1 char*
 * @param value2 int
 * @param value3 double
 * @return 0 if the insertion was successful; -1 if the insertion failed.
 */
int set_value(int key, char *value1, int value2, double value3);

/**
 * @brief Assign the values of the element associated to the key to the variables value1, value2 and value3.
 * @param key int
 * @param value1 char*
 * @param value2 int
 * @param value3 double
 * @return 0 if the element exists and the values were assigned correctly; -1 if the element does not exist, or an error occurred during communication.
 * @note Values are assigned to the variables passed as parameters.
 */
int get_value(int key, char *value1, int *value2, double *value3);

/**
 * @brief
 * @param key
 * @param value1 char*
 * @param value2 int
 * @param value3 double
 * @return 0 if the element exists and the values were modified correctly; -1 if the element does not exist, or an error occurred during communication.
 */
int modify_value(int key, char *value1, int value2, double value3);

/**
 * @brief Delete the element associated to the key.
 * @param key int
 * @return 0 if success; -1 if the element does not exist, or an error occurred during communication.
 */
int delete_key(int key);

/**
 * @brief Check if the element associated to the key exists.
 * @param key int
 * @return 1 if exists; 0 if not exists; -1 if error occurred during communication.
 */
int exist(int key);

/**
 * @brief Copy the element associated to the key1 to the key2.
 * @param key1 int
 * @param key2 int
 * @return 0 if values were copied correctly to key2; -1 if the key1 does not exist, or an error occurred during communication.
 */
int copy_key(int key1, int key2);

#endif