/*
 * proxy.c file - receiver
 * Authors: 100451339 & 100451170
 */

// ! Libraries declaration
#include <fcntl.h>    /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <mqueue.h>   /* For message queue */

#include <stdio.h>  /* For printf */
#include <stdlib.h> /* For exit */
#include <signal.h>
#include <string.h>

#include "request.h"  /* For request struct */
#include "response.h" /* For response struct */

#include "servidor.h"
#include "LinkedList.h"

#define MQ_SERVER "/mq_server" /* Queue name */

LinkedList *list;

mqd_t serverQueue;
mqd_t clientQueue;

// ! Request (Attribute declaration - send)
struct mq_attr requestAttributes = {
    .mq_flags = 0,                 // Flags (ignored for mq_open())
    .mq_maxmsg = 10,               // Max. # of messages on queue
    .mq_msgsize = sizeof(Request), // Max. message size (bytes)
    .mq_curmsgs = 0,               // # of messages currently in queue
};

// ! Response (Attribute declaration - receive)
struct mq_attr responseAttributes = {
    .mq_flags = 0,                  // Flags (ignored for mq_open())
    .mq_maxmsg = 1,                 // Max. # of messages on queue (only 1 response)
    .mq_msgsize = sizeof(Response), // Max. message size (bytes)
    .mq_curmsgs = 0,                // # of messages currently in queue
};

// ! Mutex & Condition variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int busy = FALSE;

// We use a signal handler to stop the server, forced to declare and use signum to avoid warnings
// ! Signal handler
void stopServer(int signum)
{
    printf("\n\nClosing the server queue...\n\n");

    // Close the queue
    mq_close(serverQueue);

    // Unlink the queue
    mq_unlink(MQ_SERVER);

    delete_linked_list(list);

    exit(signum);
}

void deal_with_request(Request *client_request)
{

    // * Response (message)
    Response server_response;

    printf("Message Received -> Execute operation (id %d):\n", client_request->operacion);

    // ! Section to be changed
    pthread_mutex_lock(&mutex); // Lock the mutex as the list is being modified (WRITE operation)

    switch (client_request->operacion)
    {
    case init_op:
        server_response.error_code = list_init(list);
        list_display_list(list);
        break;
    case set_value_op:
        server_response.error_code = list_set_value(client_request->key1, client_request->value1, client_request->value2, client_request->value3, list);
        list_display_list(list);
        break;
    case get_value_op:
        char value1response[256] = "";
        int *value2response = malloc(sizeof(int));
        double *value3response = malloc(sizeof(double));
        server_response.error_code = list_get_value(client_request->key1, value1response, value2response, value3response, list);
        strcpy(server_response.value1, value1response);
        server_response.value2 = *value2response;
        server_response.value3 = *value3response;
        free(value2response);
        free(value3response);
        list_display_list(list);
        break;
    case delete_key_op:
        server_response.error_code = list_delete_key(client_request->key1, list);
        list_display_list(list);
        break;
    case modify_value_op:
        server_response.error_code = list_modify_value(client_request->key1, client_request->value1, client_request->value2, client_request->value3, list);
        list_display_list(list);
        break;
    case exist_op:
        server_response.error_code = list_exist(client_request->key1, list);
        list_display_list(list);
        break;
    case copy_key_op:
        server_response.error_code = list_copy_key(client_request->key1, client_request->key2, list);
        list_display_list(list);
        break;
    }

    // * Open the queue
    serverQueue = mq_open(
        client_request->clientPID, // Queue name
        O_CREAT | O_WRONLY,        // Open flags (O_WRONLY for sender)
        S_IRUSR | S_IWUSR,         // User read/write permission
        &responseAttributes);      // Assign queue attributes

    // * Send the message
    mq_send(
        serverQueue,              // Queue descriptor
        (char *)&server_response, // Message buffer (cast to char* for POSIX)
        sizeof(Response),         // Message size
        0);                       // Message priority (not used)

    printf("Response created");

    mq_close(serverQueue); // Close the queue (free resources)

    // ! Section to be parallelized
    busy = FALSE;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL); // Exit the thread
}

int main(void)
{
    list = create_linked_list();

    // * Open the queue
    clientQueue = mq_open(
        MQ_SERVER,           // Queue name
        O_CREAT | O_RDONLY,  // Open flags (O_RDONLY for receiver)
        S_IRUSR | S_IWUSR,   // User read/write permission
        &requestAttributes); // Assign queue attributes

    printf("\nServer live");
    printf("\nWaiting for messages...\n\n");

    // If signal is received, stop the server
    signal(SIGINT, stopServer);

    // ! Thread attributes
    pthread_attr_t attr;                                         // Thread attributes
    pthread_attr_init(&attr);                                    // Initialize the attribute
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); // Set the attribute to detached

    // ! Mutex & Condition variables
    pthread_mutex_init(&mutex, NULL); // Initialize the mutex
    pthread_cond_init(&cond, NULL);   // Initialize the condition variable

    // of messages sent/set of messages received and so we dont have to force break the loop
    while (1)
    {
        // * Request (message)
        Request client_request;

        // * Receive the message
        mq_receive(
            clientQueue,             // Queue descriptor
            (char *)&client_request, // Message buffer (cast to char* for POSIX)
            sizeof(Request),         // Message size
            NULL);                   // Message priority (not used)

        // ! We create a thread for each request and execute the function deal_with_request
        pthread_t thread; // create threads to handle the requests as they come in

        pthread_create(&thread, &attr, (void *)deal_with_request, (void *)&client_request);

        // ! Wait for the thread to finish (child copies the descriptor)
        pthread_mutex_lock(&mutex); // Lock the mutex
        while (busy == TRUE)
        {
            printf("waiting");                // If the thread is busy, wait
            pthread_cond_wait(&cond, &mutex); // Wait for the condition variable
        }
        busy = TRUE;                  // Set the thread as busy
        pthread_mutex_unlock(&mutex); // Unlock the mutex

        printf(" -> Response sent!\n\n");
    }

    return 0;
}

/*
The list data structure is shared among threads, so proper synchronization mechanisms such as mutexes and condition variables must be used to avoid data races.
The busy flag seems to be used to indicate whether the critical section of code is being executed. Its usage should also be protected by a mutex.
The request and response message queues should be created with the O_CREAT flag only once, before the threads are created. Otherwise, if a thread tries to create a message queue that already exists, it will fail.
The signal handler should also protect shared resources with a mutex, to prevent data races if a signal is received while a thread is accessing those resources.
*/