/*
 * servidor.c file - receiver
 * Authors: 100451339 & 100451170
 */

// ! Libraries declaration
#include <fcntl.h>    /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <mqueue.h>   /* For message queue */

#include <stdio.h>  /* For printf */
#include <stdlib.h> /* For exit */

#include "request.h"  /* For request struct */
#include "response.h" /* For response struct */

#define MQ_SERVER "/mq_server" /* Queue name */

// ! Main function
int main(void)
{
    // * Attribute declaration (same as in cliente.c)
    struct mq_attr serverAttributes = {
        .mq_flags = 0,                 // Flags (ignored for mq_open())
        .mq_maxmsg = 10,               // Max. # of messages on queue
        .mq_msgsize = sizeof(Request), // Max. message size (bytes)
        .mq_curmsgs = 0,               // # of messages currently in queue
    };

    // * Open the queue
    mqd_t serverQueue = mq_open(
        MQ_SERVER,          // Queue name
        O_CREAT | O_RDONLY, // Open flags (O_RDONLY for receiver)
        S_IRUSR | S_IWUSR,  // User read/write permission
        &serverAttributes); // Assign queue attributes

    // * Request (message)
    Request request;

    printf("\nWaiting for messages...\n\n");

    // TODO: Done with an iterator i instead of while(1) so we can have a counter on the number
    // of messages sent/set of messages received and so we dont have to force break the loop
    while (1)
    {
        // ! RECEIVE
        // * Receive the message(s)
        mq_receive(
            serverQueue,      // Queue descriptor
            (char *)&request, // Message buffer (cast to char* for POSIX)
            sizeof(Request),  // Message size
            NULL);            // Message priority (not used)

        printf("Received message:\n");

        // * Print the message
        printf("Clave: %d | Valor1: %s | Valor2: %d | Valor3: %f | Operacion: %d | End: %s\n",
               request.clave, request.valor1, request.valor2, request.valor3,
               request.operacion, request.end ? "true" : "false");

        // ! SEND
        // * Response to the client - ACK
        Response response = {
            .respuesta = "ACK",
        };

        // * Attribute declaration (same as in cliente.c)
        struct mq_attr clientAttributes = {
            .mq_flags = 0,                  // Flags (ignored for mq_open())
            .mq_maxmsg = 1,                 // Max. # of messages on queue
            .mq_msgsize = sizeof(Response), // Max. message size (bytes)
            .mq_curmsgs = 0,                // # of messages currently in queue
        };

        // * Open the queue
        mqd_t clientQueue = mq_open(
            request.clientQueue, // Queue name (received from client)
            O_CREAT | O_WRONLY,  // Open flags (O_RDONLY for receiver)
            S_IRUSR | S_IWUSR,   // User read/write permission
            &clientAttributes);  // Assign queue attributes

        // * Send the message
        mq_send(
            clientQueue,       // Queue descriptor
            (char *)&response, // Message buffer (cast to char* for POSIX)
            sizeof(Response),  // Message size
            0);                // Message priority (not used)

        // * Close the queue - free resources
        mq_close(clientQueue);

        // ! EXIT CONDITION
        // * Exit condition
        if (request.end == 1)
        {
            break; // Break the loop and finish the program
        }
    }

    printf("\nClosing the queue...\n\n");

    // * Close the queue
    mq_close(serverQueue);

    return 0;
}