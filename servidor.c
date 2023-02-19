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

#include "request.h" /* For request struct */

#define MQ_NAME "/mq_queue" /* Queue name */

// ! Main function
int main(void)
{
    // * Attribute declaration (same as in cliente.c)
    struct mq_attr attributes = {
        .mq_flags = 0,                 // Flags (ignored for mq_open())
        .mq_maxmsg = 10,               // Max. # of messages on queue
        .mq_msgsize = sizeof(Request), // Max. message size (bytes)
        .mq_curmsgs = 0,               // # of messages currently in queue
    };

    // * Open the queue
    mqd_t queue = mq_open(
        MQ_NAME,            // Queue name
        O_CREAT | O_RDONLY, // Open flags (O_RDONLY for receiver)
        S_IRUSR | S_IWUSR,  // User read/write permission
        &attributes);       // Assign queue attributes

    // * Request (message)
    Request request;

    printf("\nWaiting for messages...\n\n");

    while (1)
    {
        // * Receive the message(s)
        mq_receive(
            queue,            // Queue descriptor
            (char *)&request, // Message buffer (cast to char* for POSIX)
            sizeof(Request),  // Message size
            NULL);            // Message priority (not used)

        printf("Received message:\n");

        // * Print the message
        printf("Clave: %d | Valor1: %s | Valor2: %d | Valor3: %f | Operacion: %d | End: %s\n",
               request.clave, request.valor1, request.valor2, request.valor3,
               request.operacion, request.end ? "true" : "false");

        // * Exit condition
        if (request.end == 1)
        {
            break; // Exit the loop if message id is 0
        }
    }

    printf("\nClosing the queue...\n\n");

    // * Close the queue
    mq_close(queue);

    // * Unlink the queue
    mq_unlink(MQ_NAME); // This prevents for the server to overflow the queue -> errors

    return 0;
}