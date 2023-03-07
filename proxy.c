/*
 * proxy.c file - receiver
 * Authors: 100451339 & 100451170
 */

// ! Libraries declaration
#include <fcntl.h>    /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <mqueue.h>   /* For message queue */

#include <signal.h>

#include "request.h"  /* For request struct */
#include "response.h" /* For response struct */

#include "servidor.h"
#include "LinkedList.h"

#define MQ_SERVER "/mq_server" /* Queue name */

LinkedList *list;

mqd_t serverQueue;
mqd_t clientQueue;

// ! Request (Attribute declaration - send)
struct mq_attr serverAttributes = {
    .mq_flags = 0,                 // Flags (ignored for mq_open())
    .mq_maxmsg = 10,               // Max. # of messages on queue
    .mq_msgsize = sizeof(Request), // Max. message size (bytes)
    .mq_curmsgs = 0,               // # of messages currently in queue
};

// ! Response (Attribute declaration - receive)
struct mq_attr clientAttributes = {
    .mq_flags = 0,                  // Flags (ignored for mq_open())
    .mq_maxmsg = 1,                 // Max. # of messages on queue (only 1 response)
    .mq_msgsize = sizeof(Response), // Max. message size (bytes)
    .mq_curmsgs = 0,                // # of messages currently in queue
};

// ! Mutex & Condition variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int busy = FALSE;

int error_code;

// We use a signal handler to stop the server, forced to declare and use signum to avoid warnings
void stopServer(int signum)
{
    printf("\nClosing the queue...\n\n");

    // * Close the queue
    mq_close(serverQueue);

    // * Unlink the queue
    mq_unlink(MQ_SERVER);

    delete_linked_list(list);

    exit(signum);
}

void deal_with_request(Request *request)
{
    // * Response (message)
    Response response;

    switch (request->operacion)
    {
    case set_value_op:
        pthread_mutex_lock(&mutex); // Lock the mutex as the list is being modified (WRITE operation)

        error_code = list_set_value(request->key1, request->value1, request->value2, request->value3, list);
        response.error_code = error_code;
        list_display_list(list);

        busy = FALSE;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        break;
    case get_value_op:
        char value1response[256] = "";
        int *value2response = malloc(sizeof(int));
        double *value3response = malloc(sizeof(double));

        error_code = list_get_value(request->key1, value1response, value2response, value3response, list);

        response.error_code = error_code;
        strcpy(response.value1, value1response);
        response.value2 = *value2response;
        response.value3 = *value3response;

        free(value2response);
        free(value3response);

        list_display_list(list);

        pthread_mutex_lock(&mutex);
        busy = FALSE;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        break;
    case delete_key_op:
        pthread_mutex_lock(&mutex); // Lock the mutex as the list is being modified (WRITE operation)

        error_code = list_delete_key(request->key1, list);
        response.error_code = error_code;
        list_display_list(list);

        busy = FALSE;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        break;
    case modify_value_op:
        pthread_mutex_lock(&mutex); // Lock the mutex as the list is being modified (WRITE operation)

        error_code = list_modify_value(request->key1, request->value1, request->value2, request->value3, list);
        response.error_code = error_code;
        list_display_list(list);

        busy = FALSE;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        break;
    case exist_op: // Read operation, no need to lock the mutex
        error_code = list_exist(request->key1, list);
        response.error_code = error_code;
        list_display_list(list);
        
        // We lock the mutex so we can modify the busy variable and signal the condition variable
        pthread_mutex_lock(&mutex);
        busy = FALSE;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        break;
    case copy_key_op:
        pthread_mutex_lock(&mutex); // Lock the mutex as the list is being modified (WRITE operation)

        error_code = list_copy_key(request->key1, request->key2, list);
        response.error_code = error_code;
        list_display_list(list);
        
        busy = FALSE;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        break;
    }

    // * Open the queue
    clientQueue = mq_open(
        "/mq_client_1",     // Queue name (received from client)
        O_CREAT | O_WRONLY, // Open flags (O_RDONLY for receiver)
        S_IRUSR | S_IWUSR,  // User read/write permission
        &clientAttributes); // Assign queue attributes

    printf("Queue opened");

    // * Send the message
    mq_send(
        clientQueue,       // Queue descriptor
        (char *)&response, // Message buffer (cast to char* for POSIX)
        sizeof(Response),  // Message size
        0);                // Message priority (not used)

    printf("Message sent");

    // * Close the queue - free resources
    mq_close(clientQueue);

    printf("Response created\n");

    pthread_exit(NULL);
}

// ! Main function
int main(void)
{
    list = create_linked_list();
    error_code = -1;

    // * Open the queue
    serverQueue = mq_open(
        MQ_SERVER,          // Queue name
        O_CREAT | O_RDONLY, // Open flags (O_RDONLY for receiver)
        S_IRUSR | S_IWUSR,  // User read/write permission
        &serverAttributes); // Assign queue attributes

    printf("\nWaiting for messages...\n\n");

    signal(SIGINT, stopServer);

    // ! Thread attributes
    pthread_attr_t attr;
    pthread_attr_init(&attr);                                    // Initialize the attribute
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); // Set the attribute to detached

    // ! Mutex & Condition variables
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // TODO: Done with an iterator i instead of while(1) so we can have a counter on the number
    // of messages sent/set of messages received and so we dont have to force break the loop
    while (1)
    {
        // * Request (message)
        Request request;

        // * Receive the message
        mq_receive(
            serverQueue,      // Queue descriptor
            (char *)&request, // Message buffer (cast to char* for POSIX)
            sizeof(Request),  // Message size
            NULL);            // Message priority (not used)

        printf("Message received!\n");

        pthread_t thread; // create threads to handle the requests as they come in

        pthread_create(&thread, &attr, (void *)deal_with_request, (void *)&request);

        /* esperar a que el hijo copie el descriptor */
        pthread_mutex_lock(&mutex);
        while (busy == TRUE)
            pthread_cond_wait(&mutex, &cond);
        busy = TRUE;
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}