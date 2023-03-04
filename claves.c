/*
 * cliente.c
 * Authors: 100451339 & 100451170
 */

#include "claves.h"

mqd_t serverQueue;
mqd_t clientQueue;

char client_qr_name[32];

// ! Request (Attribute declaration - send)
struct mq_attr requestAttributes = {
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

void get_mq_client_name(char *qr_client_name)
{
	sprintf(qr_client_name, "/mq_client_%d", getpid());
}

int value1_length(char *value)
{
	// Check if len of value 1 is more than 256 chars
	if (strlen(value) > 256)
		return -1;
	return 0;
}

int open_queues()
{
	// * Create the queue
	serverQueue = mq_open(
		MQ_SERVER,			 // Queue name
		O_CREAT | O_WRONLY,	 // Open flags (O_WRONLY for sender)
		S_IRUSR | S_IWUSR,	 // User read/write permission
		&requestAttributes); // Assign queue attributes

	if (serverQueue == -1)
	{
		printf("Error opening server queue");
		return -1; // return -1 if the queue was not created
	}

	// * Create the queue
	clientQueue = mq_open(
		client_qr_name,		  // Queue name
		O_CREAT | O_RDONLY,	  // Open flags (O_WRONLY for sender)
		S_IRUSR | S_IWUSR,	  // User read/write permission
		&responseAttributes); // Assign queue attributes

	if (clientQueue == -1)
	{
		printf("Error opening client queue");
		return -1; // return -1 if the queue was not created
	}

	return 0;
}

int close_client_queue()
{
	// * Close the client queue
	int close_clientmq_response = mq_close(clientQueue);

	if (close_clientmq_response == -1)
		return -1; // return -1 if the queue was not closed

	return 0;
}

int init()
{
	// * Obtention of the client queue name with the PID
	get_mq_client_name(client_qr_name);
	printf("\nClient queue name: %s\n\n", client_qr_name);

	// * Close the queue
	int close_servermq_response = mq_close(serverQueue);

	if (close_servermq_response == -1)
		return -1; // return -1 if the queue was not closed

	int close_clientmq_response = mq_close(clientQueue);

	if (close_clientmq_response == -1)
		return -1; // return -1 if the queue was not closed

	// * Unlink the queue, client_qr_name is removed from the system
	int unlink_response = mq_unlink(client_qr_name);

	mq_unlink(MQ_SERVER);

	if (unlink_response == -1)
		return -1; // return -1 if the queue was not unlinked

	printf("\n");

	return 0;
}

int set_value(int key, char *value1, int value2, double value3)
{
	open_queues();

	if (value1_length(value1) == -1)
		return -1;

	// ! Send the request (Request (message) declaration)
	Request client_request = {
		.key1 = key,
		.value2 = value2,
		.value3 = value3,
		.operacion = set_value_op,
	};

	strcpy(client_request.value1, value1);
	strcpy(client_request.clientPID, client_qr_name);

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

	// * Close the client queue
	if (close_client_queue() == -1)
		return -1; // return -1 if the queue was not closed

	return server_response.error_code;
}

int get_value(int key, char *value1, int *value2, double *value3)
{
	open_queues();

	// Check if the received value is bigger than 256 -> return -1
	if (value1_length(value1) == -1)
		return -1;

	// ! Send the request (Request (message) declaration)
	Request client_request = {
		.key1 = key,
		.value2 = *value2,
		.value3 = *value3,
		.operacion = get_value_op,
	};

	strcpy(client_request.value1, value1);
	strcpy(client_request.clientPID, client_qr_name);

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

	// Modify the values of the pointers of the user with the values received from the server
	strcpy(value1, server_response.value1);
	*value2 = server_response.value2;
	*value3 = server_response.value3;

	// * Close the client queue
	if (close_client_queue() == -1)
		return -1; // return -1 if the queue was not closed

	return server_response.error_code;
}

int modify_value(int key, char *value1, int value2, double value3)
{
	open_queues();

	// Si value1_length devuelve -1 -> return -1
	if (value1_length(value1) == -1)
		return -1;

	// ! Send the request (Request (message) declaration)
	Request client_request = {
		.key1 = key,
		.value2 = value2,
		.value3 = value3,
		.operacion = modify_value_op,
	};

	// TODO: PORQ FUNCIONA CON EL STRCPY Y NO CON EL ASIGNAMIENTO DIRECTO?
	// -> RESPUESTA: porque el strcpy copia el valor de value1 a request.value1, y el asignamiento directo asigna el valor de value1 a la direccion de memoria de request.value1, que es un puntero a char, por lo que no se puede asignar un valor a un puntero a char, solo a un puntero a un puntero a char
	strcpy(client_request.value1, value1);
	strcpy(client_request.clientPID, client_qr_name);

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

	// * Close the client queue
	if (close_client_queue() == -1)
		return -1; // return -1 if the queue was not closed

	return server_response.error_code;
}

int delete_key(int key)
{
	open_queues();

	// ! Send the request (Request (message) declaration)
	Request client_request = {
		.key1 = key,
		.operacion = delete_key_op,
	};

	strcpy(client_request.clientPID, client_qr_name);

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

	// * Close the client queue
	if (close_client_queue() == -1)
		return -1; // return -1 if the queue was not closed

	return server_response.error_code;
}

int exist(int key)
{
	open_queues();

	// ! Send the request (Request (message) declaration)
	Request client_request = {
		.key1 = key,
		.operacion = exist_op,
	};

	strcpy(client_request.clientPID, client_qr_name);

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

	// * Close the client queue
	if (close_client_queue() == -1)
		return -1; // return -1 if the queue was not closed

	return server_response.error_code;
}

int copy_key(int key1, int key2)
{
	open_queues();

	// ! Send the request (Request (message) declaration)
	Request client_request = {
		.key1 = key1,
		.key2 = key2,
		.operacion = copy_key_op,
	};

	strcpy(client_request.clientPID, client_qr_name);

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

	// * Close the client queue
	if (close_client_queue() == -1)
		return -1; // return -1 if the queue was not closed

	return server_response.error_code;
}
