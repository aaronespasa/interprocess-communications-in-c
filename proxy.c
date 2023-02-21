/*
 * proxy.c
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

#define MQ_SERVER "/mq_server"   /* Queue name */
#define MQ_CLIENT "/mq_client_0" /* Queue name */

// ! Main function
int main(void)
{
    // ! Request
    // * Attribute declaration - send
    struct mq_attr serverAttributes = {
        .mq_flags = 0,                 // Flags (ignored for mq_open())
        .mq_maxmsg = 10,               // Max. # of messages on queue
        .mq_msgsize = sizeof(Request), // Max. message size (bytes)
        .mq_curmsgs = 0,               // # of messages currently in queue
    };

    // * Create the queue
    mqd_t serverQueue = mq_open(
        MQ_SERVER,          // Queue name
        O_CREAT | O_WRONLY, // Open flags (O_WRONLY for sender)
        S_IRUSR | S_IWUSR,  // User read/write permission
        &serverAttributes); // Assign queue attributes

    // ! Response
    // * Attribute declaration - receive
    struct mq_attr responseAttributes = {
        .mq_flags = 0,                  // Flags (ignored for mq_open())
        .mq_maxmsg = 1,                 // Max. # of messages on queue (only 1 response)
        .mq_msgsize = sizeof(Response), // Max. message size (bytes)
        .mq_curmsgs = 0,                // # of messages currently in queue
    };

    // * Create the queue
    mqd_t clientQueue = mq_open(
        MQ_CLIENT,            // Queue name
        O_CREAT | O_RDONLY,   // Open flags (O_WRONLY for sender)
        S_IRUSR | S_IWUSR,    // User read/write permission
        &responseAttributes); // Assign queue attributes

    // ! Send the request
    // * Request (message) declaration
    Request request0 = {
        .clave = 0,
        .valor1 = "Hola",
        .valor2 = 0,
        .valor3 = 0.0,
        .operacion = init,
        .clientQueue = MQ_CLIENT,
        .end = 0, // 0 = false, 1 = true (used to terminate the loop)
    };

    Request request1 = {
        .clave = 1,
        .valor1 = "Mundo",
        .valor2 = 0,
        .valor3 = 0.1,
        .operacion = set_value,
        .clientQueue = MQ_CLIENT,
        .end = 0, // 0 = false, 1 = true (used to terminate the loop)
    };

    // * Send the message
    mq_send(
        serverQueue,       // Queue descriptor
        (char *)&request0, // Message buffer (cast to char* for POSIX)
        sizeof(Request),   // Message size
        0);                // Message priority

    mq_send(
        serverQueue,       // Queue descriptor
        (char *)&request1, // Message buffer (cast to char* for POSIX)
        sizeof(Request),   // Message size
        0);                // Message priority

    // ! Receive the response
    // * Response (message) declaration
    Response response;

    // * Receive the message
    mq_receive(
        clientQueue,       // Queue descriptor
        (char *)&response, // Message buffer (cast to char* for POSIX)
        sizeof(Response),  // Message size
        NULL);             // Message priority (not used)

    // * Print the response
    printf("Response: %s\n", response.respuesta);

    // ! Terminate the queue
    // * Close the queue
    mq_close(serverQueue);
    mq_close(clientQueue);

    // * Unlink the queue
    // With this, we cannot access the queue anymore nor send multiple times the requests
    // mq_unlink(MQ_NAME); // Unlink the queue, MQ_QUEUE is removed from the system
    mq_unlink(MQ_CLIENT); // Unlink the queue, MQ_CLIENT is removed from the system
}