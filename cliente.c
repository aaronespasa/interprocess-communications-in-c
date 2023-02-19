/*
 * Sender
 * Authors: 100451339 & 100451170
 */

// ! Libraries declaration
#include <fcntl.h>	  /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <mqueue.h>	  /* For message queue */

#include <stdio.h>	/* For printf */
#include <stdlib.h> /* For exit */

#include "request.h" /* For request struct */

#define MQ_NAME "/mq_queue" /* Queue name */

// ! Main function
int main(void)
{
	// * Attribute declaration
	struct mq_attr attributes = {
		.mq_flags = 0,				   // Flags (ignored for mq_open())
		.mq_maxmsg = 10,			   // Max. # of messages on queue
		.mq_msgsize = sizeof(Request), // Max. message size (bytes)
		.mq_curmsgs = 0,			   // # of messages currently in queue
	};

	// * Open the queue
	mqd_t queue = mq_open(
		MQ_NAME,			// Queue name
		O_WRONLY | O_CREAT, // Open flags (O_WRONLY for sender)
		S_IRUSR | S_IWUSR,	// User read/write permission
		&attributes);		// Assign queue attributes

	// * Request (message) declaration
	Request request0 = {
		.id = 1,
		.text = "Hello world!"}; // Request 0

	// * Send the message
	mq_send(
		queue,			   // Queue descriptor
		(char *)&request0, // Message buffer (cast to char* for POSIX)
		sizeof(Request),   // Message size
		0);				   // Message priority

	// * Close the queue
	mq_close(queue);

	// * Unlink the queue
	mq_unlink(MQ_NAME); // Unlink the queue, so it's destroyed
}