/*
 * servidor.c
 * Authors: 100451339 & 100451170
 */

#include "servidor.h"
#include "LinkedList.h"

// We'll use semaphores to control the access as readers/writers
#include <semaphore.h>
#include <pthread.h>

// Define true and false as 1 and 0 to avoid using the stdbool.h library
#define true 1    //  Macro to map true to 1
#define false 0   //  Macro to map false to 0
typedef int bool; // Define bool as an allias for int

// The wait() and post() semaphores are atomic -> do not assure order

sem_t writer_sem;           // semaphores for readers and writers
pthread_mutex_t reader_mut; // mutex for the reader_count variable
int reader_count = 0;       // number of readers reading
bool is_semaphore_initialized = false;
bool is_mutex_initialized = false;

void initialize_mutex_and_semaphore()
{
    // Initialize the mutex
    if (!is_mutex_initialized)
    {
        pthread_mutex_init(&reader_mut, NULL);
        is_mutex_initialized = true;
    }

    // Initialize the mutex
    if (!is_semaphore_initialized)
    {
        sem_init(&writer_sem, 0, 1);
        is_semaphore_initialized = true;
    }
}

int list_init(LinkedList *list)
{
    // Initialize the mutex and semaphore if they are not initialized
    initialize_mutex_and_semaphore();

    // Writer tries to get the write semaphore
    sem_wait(&writer_sem);

    // initialize linked list
    int error_code = init(list);

    // Writer releases the write semaphore
    sem_post(&writer_sem);

    return error_code;
}

int list_set_value(int key, char *value1, int value2, double value3, LinkedList *list)
{
    // Initialize the mutex and semaphore if they are not initialized
    initialize_mutex_and_semaphore();

    // Writer tries to get the write semaphore
    sem_wait(&writer_sem);

    // Insert request in the linked list
    int error_code = set_value(list, key, value1, value2, value3);

    // Writer releases the write semaphore
    sem_post(&writer_sem);

    return error_code;
}

int list_get_value(int key, char *value1, int *value2, double *value3, LinkedList *list)
{
    // Initialize the mutex and semaphore if they are not initialized
    initialize_mutex_and_semaphore();

    // Acquire the reader mutex
    pthread_mutex_lock(&reader_mut);

    // Increment the reader count
    reader_count++;

    // If it's the first reader, try to get the write semaphore
    if (reader_count == 1)
    {
        sem_wait(&writer_sem);
    }

    // Release the reader mutex
    pthread_mutex_unlock(&reader_mut);

    // Get request from the linked list
    int error_code = get_value(list, key, value1, value2, value3);
    printf("\nLa Key %d existe y se han obtenido sus valores: %s, %d y %f\n", key, value1, *value2, *value3);

    // Acquire the reader mutex
    pthread_mutex_lock(&reader_mut);

    // Decrement the reader count
    reader_count--;

    // If it's the last reader, release the write semaphore
    if (reader_count == 0)
    {
        sem_post(&writer_sem);
    }

    // Release the reader mutex
    pthread_mutex_unlock(&reader_mut);

    return error_code;
}

int list_modify_value(int key, char *value1, int value2, double value3, LinkedList *list)
{
    // Initialize the mutex and semaphore if they are not initialized
    initialize_mutex_and_semaphore();

    // Writer tries to get the write semaphore
    sem_wait(&writer_sem);

    // Create request
    int error_code = modify_value(list, key, value1, value2, value3);

    // Writer releases the write semaphore
    sem_post(&writer_sem);

    return error_code;
}

int list_delete_key(int key, LinkedList *list)
{
    // Initialize the mutex and semaphore if they are not initialized
    initialize_mutex_and_semaphore();

    // Writer tries to get the write semaphore
    sem_wait(&writer_sem);

    // Delete request from the linked list
    int error_code = delete_key(list, key);

    // Writer releases the write semaphore
    sem_post(&writer_sem);

    return error_code;
}

int list_exist(int key, LinkedList *list)
{
    // Initialize the mutex and semaphore if they are not initialized
    initialize_mutex_and_semaphore();

    // Acquire the reader mutex
    pthread_mutex_lock(&reader_mut);

    // Increment the reader count
    reader_count++;

    // If it's the first reader, try to get the write semaphore
    if (reader_count == 1)
    {
        sem_wait(&writer_sem);
    }

    // Release the reader mutex
    pthread_mutex_unlock(&reader_mut);

    // Check if key exists in the linked list
    int error_code = exist(list, key);

    // Acquire the reader mutex
    pthread_mutex_lock(&reader_mut);

    // Decrement the reader count
    reader_count--;

    // If it's the last reader, release the write semaphore
    if (reader_count == 0)
    {
        sem_post(&writer_sem);
    }

    // Release the reader mutex
    pthread_mutex_unlock(&reader_mut);

    return error_code;
}

int list_copy_key(int key1, int key2, LinkedList *list)
{
    // Initialize the mutex and semaphore if they are not initialized
    initialize_mutex_and_semaphore();

    // Writer tries to get the write semaphore
    sem_wait(&writer_sem);

    // Copy request from key1 to key2 in the linked list
    int error_code = copy_key(list, key1, key2);

    // Writer releases the write semaphore
    sem_post(&writer_sem);

    return error_code;
}

void list_display_list(LinkedList *list)
{
    // Initialize the mutex and semaphore if they are not initialized
    initialize_mutex_and_semaphore();
    // Acquire the reader mutex
    pthread_mutex_lock(&reader_mut);

    // Increment the reader count
    reader_count++;

    // If it's the first reader, try to get the write semaphore
    if (reader_count == 1)
    {
        sem_wait(&writer_sem);
    }

    // Release the reader mutex
    pthread_mutex_unlock(&reader_mut);

    // Display the linked list
    display_list(list);

    // Acquire the reader mutex
    pthread_mutex_lock(&reader_mut);

    // Decrement the reader count
    reader_count--;

    // If it's the last reader, release the write semaphore
    if (reader_count == 0)
    {
        sem_post(&writer_sem);
    }

    // Release the reader mutex
    pthread_mutex_unlock(&reader_mut);
}
