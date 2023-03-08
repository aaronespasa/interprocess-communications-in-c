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

    printf(" -> Generate response:\n");

    switch (client_request->operacion)
    {
    case init_op:
        printf("CASE: init_op");
        server_response.error_code = list_init(list);
        printf(" -> Response: %d (error code)\n", server_response.error_code);
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

    // * Close the queue - free resources
    mq_close(serverQueue);
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

        printf("Message received");

        deal_with_request(&client_request);

        printf(" -> Response sent!\n\n");
    }

    return 0;
}