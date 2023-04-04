/*
 * cliente.c
 * Authors: 100451339 & 100451170
 */

#include "claves.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// the following two variables are used to print the client queue name only once
int num_of_clients_printed = 0;
char client_names[NUM_THREADS][32];

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

char* get_ip_tuplas()
{
	char* ip_tuplas = getenv("IP_TUPLAS");
	if (ip_tuplas == NULL)
	{
		printf("Error: IP_TUPLAS environment variable not set\n");
		exit(1);
	}

	printf("IP_TUPLAS: %s\n", ip_tuplas);
	return ip_tuplas;
}

int get_port_tuplas()
{
	char* port_tuplas = getenv("PORT_TUPLAS");
	char *end;
	if (port_tuplas == NULL)
	{
		printf("Error: PORT_TUPLAS environment variable not set\n");
		exit(1);
	}

	int port_tuplas_int = strtol(port_tuplas, &end, 10);
	if (*end != '\0')
	{
		printf("Error: PORT_TUPLAS environment variable is not a number\n");
		exit(1);
	}

	printf("PORT_TUPLAS: %d\n", port_tuplas_int);
	return port_tuplas_int;
}

void get_mq_client_name(char *qr_client_name)
{
	sprintf(qr_client_name, "/mq_client_%lu", pthread_self());
}

int value1_length(char *value)
{
	// Check if len of value 1 is more than 256 chars
	if (strlen(value) > 256)
		return -1;
	return 0;
}

void print_client_queue_name()
{
	char client_qr_name[32];
	get_mq_client_name(client_qr_name);

	pthread_mutex_lock(&mutex);
	// check if client_qr_name is in client_names
	int client_qr_name_in_client_names = 0;
	for (int i = 0; i < NUM_THREADS; i++)
	{
		if (strcmp(client_names[i], client_qr_name) == 0)
		{
			client_qr_name_in_client_names = 1;
			break;
		}
	}

	if (num_of_clients_printed < NUM_THREADS && client_qr_name_in_client_names == 0)
	{
		printf("\nClient queue name: %s\n\n", client_qr_name);
		strcpy(client_names[num_of_clients_printed], client_qr_name);
		num_of_clients_printed++;
	}

	// print ip tuplas and port tuplas
	// if (num_of_clients_printed == NUM_THREADS)
	// {
	// 	printf("IP_TUPLAS: %s\n", get_ip_tuplas());
	// 	printf("PORT_TUPLAS: %d\n", get_port_tuplas());
	// }

	pthread_mutex_unlock(&mutex);
}

int open_queues(mqd_t* serverQueue, mqd_t* clientQueue)
{
	char client_qr_name[32];
	get_mq_client_name(client_qr_name);
	print_client_queue_name();
	

	// * Create the queue
	*serverQueue = mq_open(
		MQ_SERVER,			 // Queue name
		O_CREAT | O_WRONLY,	 // Open flags (O_WRONLY for sender)
		S_IRUSR | S_IWUSR,	 // User read/write permission
		&requestAttributes); // Assign queue attributes

	if (*serverQueue == -1)
	{
		printf("Error opening server queue");
		return -1; // return -1 if the queue was not created
	}

	// * Create the queue
	*clientQueue = mq_open(
		client_qr_name,		  // Queue name
		O_CREAT | O_RDONLY,	  // Open flags (O_WRONLY for sender)
		S_IRUSR | S_IWUSR,	  // User read/write permission
		&responseAttributes); // Assign queue attributes

	if (*clientQueue == -1)
	{
		printf("Error opening client queue");
		return -1; // return -1 if the queue was not created
	}

	return 0;
}

int init()
{
	mqd_t serverQueue, clientQueue;
	open_queues(&serverQueue, &clientQueue);

	// !Send the request(Request(message) declaration)

	Request client_request = {
		.operacion = init_op,
	};

	char client_qr_name[32];
	get_mq_client_name(client_qr_name);

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
		sizeof(Response),		  // Message size as the created response
		NULL);					  // Message priority (ignored)

	if (receive_response == -1)
		return -1; // return -1 if the message was not received

	// * Close the client queue
	if (mq_close(clientQueue) == -1)
		return -1; // return -1 if the queue was not closed

	mq_unlink(client_qr_name);

	return server_response.error_code;
}

int set_value(int key, char *value1, int value2, double value3)
{
	mqd_t serverQueue, clientQueue;
	open_queues(&serverQueue, &clientQueue);

	// Check if len of value 1 is more than 256 chars
	if (value1_length(value1) == -1)
		return -1;

	// ! Send the request (Request (message) declaration)
	Request client_request = {
		.key1 = key,
		.value2 = value2,
		.value3 = value3,
		.operacion = set_value_op,
	};

	char client_qr_name[32];
	get_mq_client_name(client_qr_name);

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
	if(mq_close(clientQueue) == -1)
		return -1; // return -1 if the queue was not closed

	mq_unlink(client_qr_name);

	return server_response.error_code;
}

int get_value(int key, char *value1, int *value2, double *value3)
{
	mqd_t serverQueue, clientQueue;
	open_queues(&serverQueue, &clientQueue);

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

	char client_qr_name[32];
	get_mq_client_name(client_qr_name);

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
	if (mq_close(clientQueue) == -1)
		return -1; // return -1 if the queue was not closed

	mq_unlink(client_qr_name);

	return server_response.error_code;
}

int modify_value(int key, char *value1, int value2, double value3)
{
	mqd_t serverQueue, clientQueue;
	open_queues(&serverQueue, &clientQueue);

	char client_qr_name[32];
	get_mq_client_name(client_qr_name);

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
	if (mq_close(clientQueue) == -1)
		return -1; // return -1 if the queue was not closed

	mq_unlink(client_qr_name);

	return server_response.error_code;
}

int delete_key(int key)
{
	mqd_t serverQueue, clientQueue;
	open_queues(&serverQueue, &clientQueue);

	char client_qr_name[32];
	get_mq_client_name(client_qr_name);

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
	if (mq_close(clientQueue) == -1)
		return -1; // return -1 if the queue was not closed

	mq_unlink(client_qr_name);

	return server_response.error_code;
}

int exist(int key)
{
	mqd_t serverQueue, clientQueue;
	open_queues(&serverQueue, &clientQueue);

	char client_qr_name[32];
	get_mq_client_name(client_qr_name);

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
	if (mq_close(clientQueue) == -1)
		return -1; // return -1 if the queue was not closed

	mq_unlink(client_qr_name);

	return server_response.error_code;
}

int copy_key(int key1, int key2)
{
	mqd_t serverQueue, clientQueue;
	open_queues(&serverQueue, &clientQueue);
	char client_qr_name[32];
	get_mq_client_name(client_qr_name);

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
	if (mq_close(clientQueue) == -1)
		return -1; // return -1 if the queue was not closed

	mq_unlink(client_qr_name);

	return server_response.error_code;
}
