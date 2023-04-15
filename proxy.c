/*
 * File: proxy.c
 * Authors: 100451339 & 100451170
 */

// ! Libraries declaration
#include <fcntl.h>      /* For O_* constants */
#include <sys/stat.h>   /* For mode constants */
#include <sys/socket.h> /* For socket(), connect(), send(), and recv() */
#include <netinet/in.h> /* For sockaddr_in and inet_addr() */
#include <arpa/inet.h>  /* For inet_addr() */
#include <stdio.h>      /* For printf */
#include <stdlib.h>     /* For exit */
#include <signal.h>     /* For signal */
#include <string.h>     /* For strlen, strcpy, sprintf */
#include <unistd.h>     /* For getpid */

#include "request.h"  /* For request struct */
#include "servidor.h" /* For server functions */
#include "lines.h"    /* For reading the lines send from a socket */

#define MAX_LINE 256

// ! Mutex & Condition variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int busy = true;

// ! Signal handler
// Using a signal handler to stop the server, forced to declare and use signum to avoid warnings
void stopServer(int signum)
{
    printf("\n\nClosing the server queue...\n\n");

    // Unlink the queue
    // mq_unlink(MQ_SERVER);

    request_delete_list();

    exit(signum);
}

/**
 * @brief Create the socket
 *
 * @param port
 * @return int
 */
int create_socket(int port)
{
    int sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd == -1)
    {
        perror("Error creating the socket");
        exit(1);
    }

    // ! Socket options
    int optval = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval)) == -1)
    {
        perror("Error setting socket options");
        exit(1);
    }

    // ! Bind the socket
    struct sockaddr_in server_addr = {0};
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error binding the socket");
        exit(1);
    }

    // ! Listen for connections
    if (listen(sd, SOMAXCONN) == -1)
    {
        perror("Error listening for connections");
        exit(1);
    }

    return sd;
}

int read_int(int sd)
{
    int int_value = -2;

    ssize_t bytes_read = read(sd, &int_value, sizeof(int));
    if (bytes_read == -1)
    {
        perror("Error reading the integer value");
        exit(1);
    }

    return int_value;
}

double read_double(int sd)
{
    double double_value = -2;

    ssize_t bytes_read = read(sd, &double_value, sizeof(double));
    if (bytes_read == -1)
    {
        perror("Error reading the double value");
        exit(1);
    }

    return double_value;
}

void send_int(int sd, int int_value)
{
	if (send(sd, &int_value, sizeof(int), 0) == -1)
	{
		printf("Error sending integer value to the server\n");
		exit(1);
	}
}

void send_double(int sd, double double_value)
{
	if (send(sd, &double_value, sizeof(double), 0) == -1)
	{
		printf("Error sending double value to the server\n");
		exit(1);
	}
}

void send_string(int sd, char* string)
{
	int len = strlen(string) + 1;

	if ((sendMessage(sd, string, len)) == -1)
	{
		printf("Error sending string to the server\n");
		exit(1);
	}
}


Parameters get_parameters(int client_sd, int operation_code) {
    int num_parameters = OPERATION_PARAMS[operation_code];

    Parameters parameters;

    if(strcmp(OPERATION_NAMES[operation_code], "copy_key") == 0) {
        parameters.key1 = read_int(client_sd);
        parameters.key2 = read_int(client_sd);
    } else {
        for (int i = 0; i < num_parameters; i++)
        {
            switch (i)
            {
            case 0:
                parameters.key1 = read_int(client_sd);
                break;
            case 1:
                readLine(client_sd, parameters.value1, MAX_LINE);
                break;
            case 2:
                parameters.value2 = read_int(client_sd);
                break;
            case 3:
                parameters.value3 = read_double(client_sd);
                break;
            }
        }
    }

    return parameters;
}

void deal_with_request(Request* client_request)
{
    char value1response[256] = "";
    int *value2response = malloc(sizeof(int));
    double *value3response = malloc(sizeof(double));

    Parameters parameters = {0};

    int operation_code_copy = 0;
    int client_sd = 0;
    // char client_IP[32] = "";
    // int client_port = 0;
    // struct sockaddr_in client_addr = {0};
    // socklen_t client_addr_len = sizeof(client_addr);

    // * Lock the mutex on the process of request copying
    pthread_mutex_lock(&mutex);
    operation_code_copy = client_request->operation_code;
    client_sd = client_request->socket;

    // // get the client IP and port
    // getpeername(client_sd, (struct sockaddr *)&client_addr, &client_addr_len);
    // strcpy(client_IP, inet_ntoa(client_addr.sin_addr));
    // client_port = ntohs(client_addr.sin_port);

    // // print the client IP and port
    // printf("IP: %s, Port: %d\n", client_IP, client_port);

    // printf("📥 Message Received -> Executing \"%s\" by %s:\n", OPERATION_NAMES[client_request_copy.operacion], client_request_copy.clientPID);
    busy = false;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    // * Response (message)
    int error_code = -1;

    switch (operation_code_copy)
    {
    case init_op:
        error_code = list_init();
        // list_display_list();
        break;

    case set_value_op:
        parameters = get_parameters(client_sd, operation_code_copy);
        error_code = list_set_value(parameters.key1, parameters.value1, parameters.value2, parameters.value3);
        // list_display_list();
        break;

    case get_value_op:
        // Initialize the memory pointed by value2response and value3response to avoid garbage values
        *value2response = 0;
        *value3response = 0.0;

        parameters = get_parameters(client_sd, operation_code_copy);

        error_code = list_get_value(parameters.key1, value1response, value2response, value3response);
        // list_display_list();
        break;

    case delete_key_op:
        parameters = get_parameters(client_sd, operation_code_copy);
        error_code = list_delete_key(parameters.key1);
        // list_display_list();
        break;

    case modify_value_op:
        parameters = get_parameters(client_sd, operation_code_copy);
        error_code = list_modify_value(parameters.key1, parameters.value1, parameters.value2, parameters.value3);
        // list_display_list();
        break;

    case exist_op:
        parameters = get_parameters(client_sd, operation_code_copy);
        error_code = list_exist(parameters.key1);
        // list_display_list();
        break;

    case copy_key_op:
        parameters = get_parameters(client_sd, operation_code_copy);
        error_code = list_copy_key(parameters.key1, parameters.key2);
        // list_display_list();
        break;
    }

    // * Send the error code to the client
    if (send(client_sd, &error_code, sizeof(int), 0) == -1)
    {
        printf("Error sending error code to the client\n");
        printf("Error code: %d\n", error_code);
        exit(1);
    }

    if(strcmp(OPERATION_NAMES[operation_code_copy], "get_value") == 0 && error_code == 0) {
        // send the value1response
        send_string(client_sd, value1response);
        // send the value2response
        send_int(client_sd, *value2response);
        // send the value3response
        send_double(client_sd, *value3response);
    }

    // close the socket
    close(client_sd);

    free(value2response);
    free(value3response);
}

/**
 * @brief Get the port number from the user
 *
 * @param argc
 * @param argv
 * @return int
 */
int process_port(int argc, char *argv[])
{
    char *end;

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // Now we are sure that the user has only entered the port
    // -> Let's check if it's an integer and then convert it to int
    int port = (int)strtol(argv[1], &end, 10);

    if (*end != '\0')
    {
        printf("Invalid port number format: %s\n", argv[1]);
        exit(1);
    }

    if (port < 1024 || port > 65535)
    {
        printf("Invalid port number: %d\n", port);
        exit(1);
    }

    return port;
}

int main(int argc, char *argv[])
{
    int port = process_port(argc, argv);
    printf("Port: %d\n", port);
    int sd = create_socket(port);

    // Register the signal handler
    signal(SIGINT, stopServer);

    printf("\nServer live");
    printf("\nWaiting for messages...\n\n");

    // If signal is received, stop the server
    signal(SIGINT, stopServer);

    // ! Thread attributes
    pthread_attr_t attr;                                         // Thread attributes
    pthread_attr_init(&attr);                                    // Initialize the attribute
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); // Set the attribute to detached

    // ! Mutex & Condition variables
    pthread_mutex_init(&mutex, NULL); // Initialize the mutex
    pthread_cond_init(&cond, NULL);   // Initialize the condition variable

    // of messages sent/set of messages received and so we dont have to force break the loop
    while (1)
    {
        // * Open the client socket
        Request client_request = {0};
        struct sockaddr_in client_addr = {0};
        socklen_t client_addr_len = sizeof(client_addr);
        int client_sd = accept(sd, (struct sockaddr *)&client_addr, &client_addr_len);

        if (client_sd == -1)
        {
            perror("Error opening the client socket");
            exit(1);
        }

        // * Request (message)
        int operation_code;
        ssize_t bytes_read = read(client_sd, &operation_code, sizeof(int));
        if (bytes_read == -1)
        {
            perror("Error reading the request");
            exit(1);
        }

        // * Create the request
        client_request.socket = client_sd;
        client_request.operation_code = operation_code;

        // ! We create a thread for each request and execute the function deal_with_request
        pthread_t thread; // create threads to handle the requests as they come in

        pthread_create(&thread, &attr, (void *)deal_with_request, (void *)&client_request);

        // ! Wait for the thread to finish (child copies the descriptor)
        pthread_mutex_lock(&mutex); // Lock the mutex
        while (busy)
        {
            pthread_cond_wait(&cond, &mutex); // Wait for the condition variable
        }
        busy = true;                  // Set the thread as busy
        pthread_mutex_unlock(&mutex); // Unlock the mutex

        // printf(" -> Response sent!\n\n");
    }

    // ! Destroy the mutex and the condition variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    close(sd);

    return 0;
}
