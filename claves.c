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

void get_mq_client_name(char* MQ_CLIENT) {
	sprintf(MQ_CLIENT, "/mq_client_%d", getpid());
}

// getpid() returns the process ID of the calling process
// /CLIENT_{getpid()} is the name of the queue
// sprintf(qr_name, "/CLIENT_%d", getpid()); // qr_name = "/CLIENT_1234"

int init() {
	// char MQ_CLIENT[32]; get_mq_client_name(MQ_CLIENT);
	char MQ_CLIENT[32] = "/mq_client_1";

	// * Create the queue
	serverQueue = mq_open(
		MQ_SERVER,			// Queue name
		O_CREAT | O_WRONLY, // Open flags (O_WRONLY for sender)
		S_IRUSR | S_IWUSR,	// User read/write permission
		&serverAttributes); // Assign queue attributes
		
	if (serverQueue == -1) return -1; // return -1 if the queue was not created

	// * Create the queue
	clientQueue = mq_open(
		MQ_CLIENT,			  // Queue name
		O_CREAT | O_RDONLY,	  // Open flags (O_WRONLY for sender)
		S_IRUSR | S_IWUSR,	  // User read/write permission
		&responseAttributes); // Assign queue attributes
	
	if (clientQueue == -1) return -1; // return -1 if the queue was not created

	return 0;
}

int set_value(int key, char *value1, int value2, double value3) {
	// char MQ_CLIENT[32]; get_mq_client_name(MQ_CLIENT);
	char MQ_CLIENT[32] = "/mq_client_1";

	if(strlen(value1) > 256) return -1;

	// ! Send the request (Request (message) declaration)
	Request request = {
		.key1 = key,
		.value2 = value2,
		.value3 = value3,
		.operacion = set_value_op,
	};

	strcpy(request.value1, value1);
	strcpy(request.clientQueue, MQ_CLIENT);

	// * Send the request to assign the value
	int send_request = mq_send(
		serverQueue,	  // Queue descriptor
		(char *)&request, // Message buffer (cast to char* for POSIX)
		sizeof(Request),  // Message size
		0);				  // Message priority
	
	if(send_request == -1) return -1; // return -1 if the message was not sent

	Response response;

	// * Receive the response
	int receive_response = mq_receive(
		clientQueue,	   // Queue descriptor
		(char *)&response, // Message buffer (cast to char* for POSIX)
		sizeof(Response),  // Message size
		NULL);			   // Message priority (not used)
	
	if(receive_response == -1) return -1; // return -1 if the message was not received

	return response.error_code;
}

int get_value(int key, char *value1, int *value2, double *value3) {
	// char MQ_CLIENT[32]; get_mq_client_name(MQ_CLIENT);
	char MQ_CLIENT[32] = "/mq_client_1";

	if(strlen(value1) > 256) return -1;

	// ! Send the request (Request (message) declaration)
	Request request = {
		.key1 = key,
		.value2 = *value2,
		.value3 = *value3,
		.operacion = get_value_op
	};

	strcpy(request.value1, value1);
	strcpy(request.clientQueue, MQ_CLIENT);

	// * Send the request to get the value
	int send_request = mq_send(
		serverQueue,		// Queue descriptor
		(char *)&request,	// Message buffer (cast to char* for POSIX)
		sizeof(Request), 	// Message size
		0);					// Message priority

	if(send_request == -1) return -1; // return -1 if the message was not sent

	Response response;

	// * Receive the response
	int receive_response = mq_receive(
		clientQueue,	   // Queue descriptor
		(char *)&response, // Message buffer (cast to char* for POSIX)
		sizeof(Response),  // Message size
		NULL);			   // Message priority (not used)
	
	if(receive_response == -1) return -1; // return -1 if the message was not received

	strcpy(value1, response.value1);
	*value2 = response.value2;
	*value3 = response.value3;

	return response.error_code;
}

int modify_value(int key, char *value1, int value2, double value3) {
	// char MQ_CLIENT[32]; get_mq_client_name(MQ_CLIENT);
	char MQ_CLIENT[32] = "/mq_client_1";

	if(strlen(value1) > 256) return -1;

	// ! Send the request (Request (message) declaration)
	Request request = {
		.key1 = key,
		.value2 = value2,
		.value3 = value3,
		.operacion = modify_value_op,
	};

	strcpy(request.value1, value1);
	strcpy(request.clientQueue, MQ_CLIENT);

	// * Send the request to modify the value
	int send_request = mq_send(
		serverQueue,	  // Queue descriptor
		(char *)&request, // Message buffer (cast to char* for POSIX)
		sizeof(Request),  // Message size
		0);				  // Message priority
	
	if(send_request == -1) return -1; // return -1 if the message was not sent

	Response response;

	// * Receive the response
	int receive_response = mq_receive(
		clientQueue,	   // Queue descriptor
		(char *)&response, // Message buffer (cast to char* for POSIX)
		sizeof(Response),  // Message size
		NULL);			   // Message priority (not used)
	
	if(receive_response == -1) return -1; // return -1 if the message was not received

	return response.error_code;
}

int delete_key(int key) {
	// char MQ_CLIENT[32]; get_mq_client_name(MQ_CLIENT);
	char MQ_CLIENT[32] = "/mq_client_1";

	// ! Send the request (Request (message) declaration)
	Request request = {
		.key1 = key,
		.operacion = delete_key_op,
	};

	strcpy(request.clientQueue, MQ_CLIENT);

	// * Send the request to delete the key
	int send_request = mq_send(
		serverQueue,	  // Queue descriptor
		(char *)&request, // Message buffer (cast to char* for POSIX)
		sizeof(Request),  // Message size
		0);				  // Message priority
	
	if(send_request == -1) return -1; // return -1 if the message was not sent

	Response response;

	// * Receive the response
	int receive_response = mq_receive(
		clientQueue,	   // Queue descriptor
		(char *)&response, // Message buffer (cast to char* for POSIX)
		sizeof(Response),  // Message size
		NULL);			   // Message priority (not used)
	
	if(receive_response == -1) return -1; // return -1 if the message was not received

	return response.error_code;
}

int exist(int key) {
	// char MQ_CLIENT[32]; get_mq_client_name(MQ_CLIENT);
	char MQ_CLIENT[32] = "/mq_client_1";

	// ! Send the request (Request (message) declaration)
	Request request = {
		.key1 = key,
		.operacion = exist_op,
	};

	strcpy(request.clientQueue, MQ_CLIENT);

	// * Send the request to check if the key exists
	int send_request = mq_send(
		serverQueue,	  // Queue descriptor
		(char *)&request, // Message buffer (cast to char* for POSIX)
		sizeof(Request),  // Message size
		0);				  // Message priority
	
	if(send_request == -1) return -1; // return -1 if the message was not sent

	Response response;

	// * Receive the response
	int receive_response = mq_receive(
		clientQueue,	   // Queue descriptor
		(char *)&response, // Message buffer (cast to char* for POSIX)
		sizeof(Response),  // Message size
		NULL);			   // Message priority (not used)
	
	if(receive_response == -1) return -1; // return -1 if the message was not received

	return response.error_code;
}

int copy_key(int key1, int key2) {
	// char MQ_CLIENT[32]; get_mq_client_name(MQ_CLIENT);
	char MQ_CLIENT[32] = "/mq_client_1";

	// ! Send the request (Request (message) declaration)
	Request request = {
		.key1 = key1,
		.key2 = key2,
		.operacion = copy_key_op,
	};

	strcpy(request.clientQueue, MQ_CLIENT);

	// * Send the request to copy the key
	int send_request = mq_send(
		serverQueue,	  // Queue descriptor
		(char *)&request, // Message buffer (cast to char* for POSIX)
		sizeof(Request),  // Message size
		0);				  // Message priority
	
	if(send_request == -1) return -1; // return -1 if the message was not sent

	Response response;

	// * Receive the response
	int receive_response = mq_receive(
		clientQueue,	   // Queue descriptor
		(char *)&response, // Message buffer (cast to char* for POSIX)
		sizeof(Response),  // Message size
		NULL);			   // Message priority (not used)
	
	if(receive_response == -1) return -1; // return -1 if the message was not received

	return response.error_code;
}

int close_queue() {
	// char MQ_CLIENT[32]; get_mq_client_name(MQ_CLIENT);
	char MQ_CLIENT[32] = "/mq_client_1";

	// * Close the queue
	int close_servermq_response = mq_close(serverQueue);
	
	if(close_servermq_response == -1) return -1; // return -1 if the queue was not closed

	int close_clientmq_response = mq_close(clientQueue);

	if(close_clientmq_response == -1) return -1; // return -1 if the queue was not closed

	// * Unlink the queue, MQ_CLIENT is removed from the system
	int unlink_response = mq_unlink(MQ_CLIENT);
	
	if(unlink_response == -1) return -1; // return -1 if the queue was not unlinked

	return 0;
}
