/*
 * cliente.c
 * Authors: 100451339 & 100451170
 */

#include "claves.h"

mqd_t serverQueue;
mqd_t clientQueue;

// ! Request (Attribute declaration - send)
struct mq_attr serverAttributes = {
	.mq_flags = 0,				   // Flags (ignored for mq_open())
	.mq_maxmsg = 10,			   // Max. # of messages on queue
	.mq_msgsize = sizeof(Request), // Max. message size (bytes)
	.mq_curmsgs = 0,			   // # of messages currently in queue
};

// ! Response (Attribute declaration - receive)
struct mq_attr responseAttributes = {
	.mq_flags = 0,					// Flags (ignored for mq_open())
	.mq_maxmsg = 1,					// Max. # of messages on queue (only 1 response)
	.mq_msgsize = sizeof(Response), // Max. message size (bytes)
	.mq_curmsgs = 0,				// # of messages currently in queue
};

// getpid() returns the process ID of the calling process
// sprintf(qr_name, "/CLIENT_%d", getpid()); // qr_name = "/CLIENT_1234"
// /CLIENT_{getpid()} is the name of the queue
void get_mq_client_name(char *MQ_CLIENT)
{
	sprintf(MQ_CLIENT, "/mq_client_%d", getpid());
}

int value1_length(char *value)
{
	// Check if len of value 1 is more than 256 chars
	if (strlen(value) > 256)
		return -1;
	return 0;
}

int init()
{
	// char MQ_CLIENT[32]; get_mq_client_name(MQ_CLIENT);
	char MQ_CLIENT[32] = "/mq_client_1";

	// * Create the queue
	serverQueue = mq_open(
		MQ_SERVER,			// Queue name
		O_CREAT | O_WRONLY, // Open flags (O_WRONLY for sender)
		S_IRUSR | S_IWUSR,	// User read/write permission
		&serverAttributes); // Assign queue attributes

	if (serverQueue == -1)
		return -1; // return -1 if the queue was not created

	// * Create the queue
	clientQueue = mq_open(
		MQ_CLIENT,			  // Queue name
		O_CREAT | O_RDONLY,	  // Open flags (O_WRONLY for sender)
		S_IRUSR | S_IWUSR,	  // User read/write permission
		&responseAttributes); // Assign queue attributes

	if (clientQueue == -1)
		return -1; // return -1 if the queue was not created

	return 0;
}

int set_value(int key, char *value1, int value2, double value3)
{
	// char MQ_CLIENT[32]; get_mq_client_name(MQ_CLIENT);
	char MQ_CLIENT[32] = "/mq_client_1";

	if (value1_length(value1) == -1)
		return -1;

	// ! Send the request (Request (message) declaration)
	Request client_request = {
		.key1 = key,
		.value1 = *value1,
		.value2 = value2,
		.value3 = value3,
		.operacion = set_value_op,
		.clientQueue = *MQ_CLIENT,
	};

	// strcpy(request.value1, value1);
	// strcpy(request.clientQueue, MQ_CLIENT);

	// * Send the request to assign the value
	int send_request = mq_send(
		serverQueue,			 // Queue descriptor
		(char *)&client_request, // Message buffer (cast to char* for POSIX)
		sizeof(Request),		 // Message size as the created request
		0);						 // Message priority

	if (send_request == -1)
		return -1; // return -1 if the message was not sent

	Response server_response;

	// * Receive the response
	int receive_response = mq_receive(
		clientQueue,			  // Queue descriptor
		(char *)&server_response, // Message buffer (cast to char* for POSIX)
		sizeof(Response),		  // Message size
		NULL);					  // Message priority (not used)

	if (receive_response == -1)
		return -1; // return -1 if the message was not received

	return server_response.error_code;
}

int get_value(int key, char *value1, int *value2, double *value3)
{
	// char MQ_CLIENT[32]; get_mq_client_name(MQ_CLIENT);
	char MQ_CLIENT[32] = "/mq_client_1";

	// Check if the received value is bigger than 256 -> return -1
	if (value1_length(value1) == -1)
		return -1;

	// ! Send the request (Request (message) declaration)
	Request client_request = {
		.key1 = key,
		.value1 = *value1,
		.value2 = *value2,
		.value3 = *value3,
		.operacion = get_value_op,
		.clientQueue = *MQ_CLIENT,
	};

	// strcpy(request.value1, value1);
	// strcpy(request.clientQueue, MQ_CLIENT);

	// * Send the request to get the value
	int send_request = mq_send(
		serverQueue,			 // Queue descriptor
		(char *)&client_request, // Message buffer (cast to char* for POSIX)
		sizeof(Request),		 // Message size
		0);						 // Message priority

	if (send_request == -1)
		return -1; // return -1 if the message was not sent

	Response server_response;

	// * Receive the response
	int receive_response = mq_receive(
		clientQueue,			  // Queue descriptor
		(char *)&server_response, // Message buffer (cast to char* for POSIX)
		sizeof(Response),		  // Message size
		NULL);					  // Message priority (not used)

	if (receive_response == -1)
		return -1; // return -1 if the message was not received

	// strcpy(value1, server_response.value1);
	// *value2 = response.value2;
	// *value3 = response.value3;

	return server_response.error_code;
}

int modify_value(int key, char *value1, int value2, double value3)
{
	// char MQ_CLIENT[32]; get_mq_client_name(MQ_CLIENT);
	char MQ_CLIENT[32] = "/mq_client_1";

	// Si value1_length devuelve -1 -> return -1
	if (value1_length(value1) == -1)
		return -1;

	// ! Send the request (Request (message) declaration)
	Request client_request = {
		.key1 = key,
		.value1 = *value1,
		.value2 = value2,
		.value3 = value3,
		.operacion = modify_value_op,
		.clientQueue = *MQ_CLIENT,
	};

	// strcpy(request.value1, value1);
	// strcpy(request.clientQueue, MQ_CLIENT);

	// * Send the request to modify the value
	int send_request = mq_send(
		serverQueue,			 // Queue descriptor
		(char *)&client_request, // Message buffer (cast to char* for POSIX)
		sizeof(Request),		 // Message size
		0);						 // Message priority

	if (send_request == -1)
		return -1; // return -1 if the message was not sent

	Response server_response;

	// * Receive the response
	int receive_response = mq_receive(
		clientQueue,			  // Queue descriptor
		(char *)&server_response, // Message buffer (cast to char* for POSIX)
		sizeof(Response),		  // Message size
		NULL);					  // Message priority (not used)

	if (receive_response == -1)
		return -1; // return -1 if the message was not received

	return server_response.error_code;
}

int delete_key(int key)
{
	// char MQ_CLIENT[32]; get_mq_client_name(MQ_CLIENT);
	char MQ_CLIENT[32] = "/mq_client_1";

	// ! Send the request (Request (message) declaration)
	Request client_request = {
		.key1 = key,
		.operacion = delete_key_op,
		.clientQueue = *MQ_CLIENT,
	};

	// strcpy(request.clientQueue, MQ_CLIENT);

	// * Send the request to delete the key
	int send_request = mq_send(
		serverQueue,			 // Queue descriptor
		(char *)&client_request, // Message buffer (cast to char* for POSIX)
		sizeof(Request),		 // Message size
		0);						 // Message priority

	if (send_request == -1)
		return -1; // return -1 if the message was not sent

	Response server_response;

	// * Receive the server_response
	int receive_response = mq_receive(
		clientQueue,			  // Queue descriptor
		(char *)&server_response, // Message buffer (cast to char* for POSIX)
		sizeof(Response),		  // Message size
		NULL);					  // Message priority (not used)

	if (receive_response == -1)
		return -1; // return -1 if the message was not received

	return server_response.error_code;
}

int exist(int key)
{
	// char MQ_CLIENT[32]; get_mq_client_name(MQ_CLIENT);
	char MQ_CLIENT[32] = "/mq_client_1";

	// ! Send the request (Request (message) declaration)
	Request client_request = {
		.key1 = key,
		.operacion = exist_op,
		.clientQueue = *MQ_CLIENT,
	};

	// strcpy(request.clientQueue, MQ_CLIENT);

	// * Send the request to check if the key exists
	int send_request = mq_send(
		serverQueue,			 // Queue descriptor
		(char *)&client_request, // Message buffer (cast to char* for POSIX)
		sizeof(Request),		 // Message size
		0);						 // Message priority

	if (send_request == -1)
		return -1; // return -1 if the message was not sent

	Response server_response;

	// * Receive the server_response
	int receive_response = mq_receive(
		clientQueue,			  // Queue descriptor
		(char *)&server_response, // Message buffer (cast to char* for POSIX)
		sizeof(Response),		  // Message size
		NULL);					  // Message priority (not used)

	if (receive_response == -1)
		return -1; // return -1 if the message was not received

	return server_response.error_code;
}

int copy_key(int key1, int key2)
{
	// char MQ_CLIENT[32]; get_mq_client_name(MQ_CLIENT);
	char MQ_CLIENT[32] = "/mq_client_1";

	// ! Send the request (Request (message) declaration)
	Request client_request = {
		.key1 = key1,
		.key2 = key2,
		.operacion = copy_key_op,
		.clientQueue = *MQ_CLIENT,
	};

	// strcpy(client_request.clientQueue, MQ_CLIENT);

	// * Send the request to copy the key
	int send_request = mq_send(
		serverQueue,			 // Queue descriptor
		(char *)&client_request, // Message buffer (cast to char* for POSIX)
		sizeof(Request),		 // Message size
		0);						 // Message priority

	if (send_request == -1)
		return -1; // return -1 if the message was not sent

	Response server_response;

	// * Receive the server_response
	int receive_response = mq_receive(
		clientQueue,			  // Queue descriptor
		(char *)&server_response, // Message buffer (cast to char* for POSIX)
		sizeof(Response),		  // Message size
		NULL);					  // Message priority (not used)

	if (receive_response == -1)
		return -1; // return -1 if the message was not received

	return server_response.error_code;
}

int close_queue()
{
	// char MQ_CLIENT[32]; get_mq_client_name(MQ_CLIENT);
	char MQ_CLIENT[32] = "/mq_client_1";

	// * Close the queue
	int close_servermq_response = mq_close(serverQueue);

	if (close_servermq_response == -1)
		return -1; // return -1 if the queue was not closed

	int close_clientmq_response = mq_close(clientQueue);

	if (close_clientmq_response == -1)
		return -1; // return -1 if the queue was not closed

	// * Unlink the queue, MQ_CLIENT is removed from the system
	int unlink_response = mq_unlink(MQ_CLIENT);

	if (unlink_response == -1)
		return -1; // return -1 if the queue was not unlinked

	return 0;
}