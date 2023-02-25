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

#include "servidor.c"

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

// We use a signal handler to stop the server, forced to declare and use signum to avoid warnings
// ! Signal handler
void stopServer(int signum)
{
    printf("\nClosing the queue...\n\n");

    // Close the queue
    mq_close(serverQueue);

    // Unlink the queue
    mq_unlink(MQ_SERVER);

    delete_linked_list(list);

    exit(signum);
}

// ! Main function
int main(void)
{
    list = create_linked_list();
    int error_code = -1;

    // * Open the queue
    serverQueue = mq_open(
        MQ_SERVER,          // Queue name
        O_CREAT | O_RDONLY, // Open flags (O_RDONLY for receiver)
        S_IRUSR | S_IWUSR,  // User read/write permission
        &serverAttributes); // Assign queue attributes

    printf("\nWaiting for messages...\n\n");

    // If signal is received, stop the server
    signal(SIGINT, stopServer);

    // TODO: Done with an iterator i instead of while(1) so we can have a counter on the number
    // of messages sent/set of messages received and so we dont have to force break the loop
    while (1)
    {
        // * Request (message)
        Request client_request;

        // * Receive the message
        mq_receive(
            serverQueue,             // Queue descriptor
            (char *)&client_request, // Message buffer (cast to char* for POSIX)
            sizeof(Request),         // Message size
            NULL);                   // Message priority (not used)

        printf("Message received!\n");

        // * Response (message)
        Response server_response;

        switch (client_request.operacion)
        {
        case set_value_op:
            error_code = list_set_value(client_request.key1, client_request.value1, client_request.value2, client_request.value3, list);
            server_response.error_code = error_code;
            list_display_list(list);
            break;

        // case get_value_op:
        //     char value1response[256] = "";
        //     int *value2response = malloc(sizeof(int));
        //     int *value3response = malloc(sizeof(int));

        //     error_code = list_get_value(client_request.key1, value1response, value2response, value3response, list);

        //     server_response.error_code = error_code;
        //     strcpy(server_response.value1, value1response);
        //     server_response.value2 = *value2response;
        //     server_response.value3 = *value3response;

        //     free(value2response);
        //     free(value3response);

        //     list_display_list(list);
        //     break;

        // case delete_key_op:
        //     error_code = list_delete_key(client_request.key1, list);
        //     server_response.error_code = error_code;
        //     list_display_list(list);
        //     break;

        // case modify_value_op:
        //     error_code = list_modify_value(client_request.key1, client_request.value1, client_request.value2, client_request.value3, list);
        //     server_response.error_code = error_code;
        //     list_display_list(list);
        //     break;

        // case exist_op:
        //     error_code = list_exist(client_request.key1, list);
        //     server_response.error_code = error_code;
        //     list_display_list(list);
        //     break;

        // case copy_key_op:
        //     error_code = list_copy_key(client_request.key1, client_request.key2, list);
        //     server_response.error_code = error_code;
        //     list_display_list(list);
        //     break;
        }

        printf("Response created\n");

        // * Open the queue
        clientQueue = mq_open(
            "/mq_client_1",     // Queue name (received from client)
            O_CREAT | O_WRONLY, // Open flags (O_RDONLY for receiver)
            S_IRUSR | S_IWUSR,  // User read/write permission
            &clientAttributes); // Assign queue attributes

        printf("Queue opened");

        // * Send the message
        mq_send(
            clientQueue,              // Queue descriptor
            (char *)&server_response, // Message buffer (cast to char* for POSIX)
            sizeof(Response),         // Message size
            0);                       // Message priority (not used)

        printf("Message sent");

        // * Close the queue - free resources
        mq_close(clientQueue);
    }

    return 0;
}