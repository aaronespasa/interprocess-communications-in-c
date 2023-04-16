/*
 * servidor.c
 * Authors: 100451339 & 100451170
 */

#include "servidor.h"

// We'll use semaphores to control the access as readers/writers
#include <semaphore.h>
#include <pthread.h>

LinkedList *list = NULL;

// The wait() and post() semaphores are atomic -> do not assure order

sem_t writer_sem;                                       // semaphores for readers and writers
pthread_mutex_t reader_mut = PTHREAD_MUTEX_INITIALIZER; // mutex for the reader_count variable and its initialization
int reader_count = 0;                                   // number of readers reading
bool is_semaphore_initialized = false;                  // semaphore initialization flag
bool is_list_created = false;                           // linked list creation flag

void init_sem()
{
    // Initialize the semaphore
    pthread_mutex_lock(&reader_mut);
    if (!is_list_created)
    {
        list = create_linked_list();
        is_list_created = true;
    }

    if (!is_semaphore_initialized)
    {
        sem_init(&writer_sem, 0, 1);
        is_semaphore_initialized = true;
    }
    pthread_mutex_unlock(&reader_mut);
}

int list_init()
{
    // Initialize the semaphore if it is not initialized
    init_sem();

    // Writer tries to get the write semaphore
    sem_wait(&writer_sem);

    // initialize linked list
    int error_code = init(list);

    // Writer releases the write semaphore
    sem_post(&writer_sem);

    return error_code;
}

int list_set_value(int key, char *value1, int value2, double value3)
{
    // Initialize the semaphore if it is not initialized
    init_sem();

    // Writer tries to get the write semaphore
    sem_wait(&writer_sem);

    // Insert request in the linked list
    int error_code = set_value(list, key, value1, value2, value3);

    // Writer releases the write semaphore
    sem_post(&writer_sem);

    return error_code;
}

int list_get_value(int key, char *value1, int *value2, double *value3)
{
    // Initialize the semaphore if it is not initialized
    init_sem();

    // Acquire the reader mutex
    pthread_mutex_lock(&reader_mut);

    // Increment the reader count
    reader_count++;

    // If it's the first reader, try to get the write semaphore
    if (reader_count == 1)
    {
        if (sem_trywait(&writer_sem) != 0)
        {
            pthread_mutex_unlock(&reader_mut);
            sem_post(&writer_sem);
            return -1; // return error code indicating that the lock was not acquired
        }
    }

    // Release the reader mutex
    pthread_mutex_unlock(&reader_mut);

    // Get request from the linked list
    int error_code = -1;
    if (list != NULL)
    {
        error_code = get_value(list, key, value1, value2, value3);
    }
    else
    {
        error_code = -1;
    }
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

int list_modify_value(int key, char *value1, int value2, double value3)
{
    // Initialize the semaphore if it is not initialized
    init_sem();

    // Writer tries to get the write semaphore
    sem_wait(&writer_sem);

    // Create request
    int error_code = modify_value(list, key, value1, value2, value3);

    // Writer releases the write semaphore
    sem_post(&writer_sem);

    return error_code;
}

int list_delete_key(int key)
{
    // Initialize the semaphore if it is not initialized
    init_sem();

    // Writer tries to get the write semaphore
    sem_wait(&writer_sem);

    // Delete request from the linked list
    int error_code = delete_key(list, key);

    // Writer releases the write semaphore
    sem_post(&writer_sem);

    return error_code;
}

int list_exist(int key)
{
    // Initialize the semaphore if it is not initialized
    init_sem();

    // Acquire the reader mutex
    pthread_mutex_lock(&reader_mut);

    // Increment the reader count
    reader_count++;

    // If it's the first reader, try to get the write semaphore
    if (reader_count == 1)
    {
        if (sem_trywait(&writer_sem) != 0)
        {
            pthread_mutex_unlock(&reader_mut);
            sem_post(&writer_sem);
            return -1; // return error code indicating that the lock was not acquired
        }
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

int list_copy_key(int key1, int key2)
{
    // Initialize the semaphore if it is not initialized
    init_sem();

    // Writer tries to get the write semaphore
    sem_wait(&writer_sem);

    // Copy request from key1 to key2 in the linked list
    int error_code = copy_key(list, key1, key2);

    // Writer releases the write semaphore
    sem_post(&writer_sem);

    return error_code;
}

int list_display_list()
{
    // Initialize the semaphore if it is not initialized
    init_sem();

    // Writer tries to get the write semaphore
    sem_wait(&writer_sem);

    // Display the linked list
    display_list(list);

    // Writer releases the write semaphore
    sem_post(&writer_sem);

    return 0;
}

void request_delete_list()
{
    if (list == NULL)
    {
        return;
    }
    delete_linked_list(list);
}
