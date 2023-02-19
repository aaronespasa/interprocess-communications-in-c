/*
 * Receiver
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

    while (1)
    {
        // * Receive the message
        mq_receive(
            queue,            // Queue descriptor
            (char *)&request, // Message buffer (cast to char* for POSIX)
            sizeof(Request),  // Message size
            NULL);            // Message priority (not used)

        // * Print the message
        printf("Received: %s (%d)\n", request.text, request.id);

        // * Exit condition
        if (request.id == 0)
        {
            break;
        }
    }

    // * Close the queue
    mq_close(queue);

    return 0;
}