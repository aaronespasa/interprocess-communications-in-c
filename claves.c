/*
 * cliente.c
 * Authors: 100451339 & 100451170
 */

#include "claves.h"

#define localhost "127.0.0.1"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

char* get_ip_tuplas()
{
	char* ip_tuplas = getenv("IP_TUPLAS");
	if (ip_tuplas == NULL)
	{
		printf("Error: IP_TUPLAS environment variable not set\n");
		exit(1);
	}

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

	return port_tuplas_int;
}

int value1_length(char *value)
{
	// Check if len of value 1 is more than 256 chars
	if (strlen(value) > 256)
		return -1;
	return 0;
}

int create_and_connect_socket()
{
	int sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sd == -1)
	{
		printf("Error creating socket\n");
		exit(1);
	}

	struct sockaddr_in server_addr = {0};
	bzero((char *)&server_addr, sizeof(server_addr));

	char* ip_tuplas = get_ip_tuplas();
	if(strcmp(ip_tuplas, "localhost") == 0)
		strcpy(ip_tuplas, localhost);
	
	int port_tuplas = get_port_tuplas();
	server_addr.sin_addr.s_addr = inet_addr(ip_tuplas);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_tuplas);

	if (connect(sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		printf("Error connecting to server\n");
		// print server_addr.sin_port and server_addr.sin_addr.s_addr
		printf("IP Tuplas: %s\n", ip_tuplas);
		printf("Port: %d\n", ntohs(server_addr.sin_port));
		printf("IP: %s\n", inet_ntoa(server_addr.sin_addr));
		exit(1);
	}

	return sd;
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

int read_int(int sd)
{
	int error_code = -2;

	ssize_t bytes_read = read(sd, &error_code, sizeof(int));
	if (bytes_read == -1)
	{
		perror("Error reading the integer value");
		exit(1);
	}

	return error_code;
}

void read_string(int sd, char string[256])
{
	if((readLine(sd, string, 256)) == -1) {
		printf("Error reading the string value");
		exit(1);
	}
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


int init()
{
	// create the socket, connect to the server
	int sd = create_and_connect_socket();
	// send the init operation
	send_int(sd, init_op);
	// now get the response (error code)
	int error_code = read_int(sd);
	// close the socket
	close(sd);
	// return the error code
	return error_code;
}

int set_value(int key, char *value1, int value2, double value3)
{
	// Check if len of value 1 is more than 256 chars
	if (value1_length(value1) == -1)
		return -1;

	// create the socket, connect to the server
	int sd = create_and_connect_socket();
	// send the set value operation
	send_int(sd, set_value_op);
	// send the key
	send_int(sd, key);
	// send the value1
	send_string(sd, value1);
	// send the value2
	send_int(sd, value2);
	// send the value3
	send_double(sd, value3);

	// now get the response (error code)
	int error_code = read_int(sd);
	// close the socket
	close(sd);
	// return the error code
	return error_code;
}

int get_value(int key, char *value1, int *value2, double *value3)
{
	// Check if the received value is bigger than 256 -> return -1
	if (value1_length(value1) == -1)
		return -1;
		
	// create the socket, connect to the server
	int sd = create_and_connect_socket();
	// send the init operation
	send_int(sd, get_value_op);
	// send the key
	send_int(sd, key);
	// now get the response (error code)
	int error_code = read_int(sd);
	// if the error code is 0, then read the values
	if (error_code == 0)
	{
		// read the value1
		read_string(sd, value1);
		// read the value2
		*value2 = read_int(sd);
		// read the value3
		*value3 = read_double(sd);
	}

	// close the socket
	close(sd);
	// return the error code
	return error_code;
}

int modify_value(int key, char *value1, int value2, double value3)
{
	// Si value1_length devuelve -1 -> return -1
	if (value1_length(value1) == -1)
		return -1;

		// create the socket, connect to the server
	int sd = create_and_connect_socket();
	// send the set value operation
	send_int(sd, modify_value_op);
	// send the key
	send_int(sd, key);
	// send the value1
	send_string(sd, value1);
	// send the value2
	send_int(sd, value2);
	// send the value3
	send_double(sd, value3);

	// now get the response (error code)
	int error_code = read_int(sd);
	// close the socket
	close(sd);
	// return the error code
	return error_code;
}

int delete_key(int key)
{
	// create the socket, connect to the server
	int sd = create_and_connect_socket();
	// send the init operation
	send_int(sd, delete_key_op);
	// send the key
	send_int(sd, key);
	// now get the response (error code)
	int error_code = read_int(sd);

	// close the socket
	close(sd);
	// return the error code
	return error_code;
}

int exist(int key)
{
	// create the socket, connect to the server
	int sd = create_and_connect_socket();
	// send the init operation
	send_int(sd, exist_op);
	// send the key
	send_int(sd, key);
	// now get the response (error code)
	int error_code = read_int(sd);

	// close the socket
	close(sd);
	// return the error code
	return error_code;
}

int copy_key(int key1, int key2)
{
	// create the socket, connect to the server
	int sd = create_and_connect_socket();
	// send the init operation
	send_int(sd, copy_key_op);
	// send the key1
	send_int(sd, key1);
	// send the key2
	send_int(sd, key2);
	// now get the response (error code)
	int error_code = read_int(sd);

	// close the socket
	close(sd);
	// return the error code
	return error_code;
}
