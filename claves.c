/*
 * cliente.c
 * Authors: 100451339 & 100451170
 */

#include "claves.h"

#define localhost "127.0.0.1"

char *get_ip_tuplas()
{
	char *ip_tuplas = getenv("IP_TUPLAS");
	if (ip_tuplas == NULL)
	{
		printf("Error: IP_TUPLAS environment variable not set\n");
		exit(1);
	}

	return ip_tuplas;
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
	char *ip_tuplas = get_ip_tuplas();
	if (strcmp(ip_tuplas, "localhost") == 0)
		strcpy(ip_tuplas, localhost);

	CLIENT *clnt = clnt_create(ip_tuplas, TUPLE_SERVICE, TUPLE_SERVICE_V1, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror(ip_tuplas);
		exit(1);
	}

	int result = -1;

	enum clnt_stat ret = RPC_FAILED;

	ret = init_clnt(&result, clnt);
	
	if (ret != RPC_SUCCESS)
	{
		clnt_perror(clnt, "call failed");
		return -1;
	}

	clnt_destroy(clnt);

	return result;
}

int set_value(int key, char *value1, int value2, double value3)
{
	// Si value1_length devuelve -1 -> return -1
	if (value1_length(value1) == -1)
		return -1;

	char *ip_tuplas = get_ip_tuplas();
	if (strcmp(ip_tuplas, "localhost") == 0)
		strcpy(ip_tuplas, localhost);
	CLIENT *clnt = clnt_create(ip_tuplas, TUPLE_SERVICE, TUPLE_SERVICE_V1, "tcp");
	if (clnt == NULL)
	{
		clnt_pcreateerror(ip_tuplas);
		exit(1);
	}

	int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	Value set_value_value;
	set_value_value.key = key;
	set_value_value.value1 = value1;
	set_value_value.value2 = value2;
	set_value_value.value3 = value3;

	ret = set_value_clnt(set_value_value, &result, clnt);

	if (ret != RPC_SUCCESS) {
		clnt_perror(clnt, "call failed");
		return ret;
	}

	clnt_destroy(clnt);

	// return the error code
	return result;
}

int get_value(int key, char *value1, int *value2, double *value3)
{
	// Si value1_length devuelve -1 -> return -1
	if (value1_length(value1) == -1)
		return -1;

	char *ip_tuplas = get_ip_tuplas();
	if (strcmp(ip_tuplas, "localhost") == 0)
		strcpy(ip_tuplas, localhost);

	CLIENT *clnt = clnt_create(ip_tuplas, TUPLE_SERVICE, TUPLE_SERVICE_V1, "tcp");
	if (clnt == NULL)
	{
		clnt_pcreateerror(ip_tuplas);
		exit(1);
	}

	enum clnt_stat ret = RPC_FAILED;

	char empty_string[256] = "";
	Value result;
	result.key = -1;
	result.value1 = empty_string;
	result.value2 = 0;
	result.value3 = 0.0;

	ret = get_value_clnt(key, &result, clnt);
	
	if (ret != RPC_SUCCESS) {
		clnt_perror(clnt, "call failed");
		return ret;
	}

	strcpy(value1, result.value1);
	*value2 = result.value2;
	*value3 = result.value3;

	clnt_destroy(clnt);

	// return the error code
	return result.key;
}

int modify_value(int key, char *value1, int value2, double value3)
{
	// Si value1_length devuelve -1 -> return -1
	if (value1_length(value1) == -1)
		return -1;

	char *ip_tuplas = get_ip_tuplas();
	if (strcmp(ip_tuplas, "localhost") == 0)
		strcpy(ip_tuplas, localhost);

	CLIENT *clnt = clnt_create(ip_tuplas, TUPLE_SERVICE, TUPLE_SERVICE_V1, "tcp");
	if (clnt == NULL)
	{
		clnt_pcreateerror(ip_tuplas);
		exit(1);
	}

	int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	Value modify_value_value;
	modify_value_value.key = key;
	modify_value_value.value1 = value1;
	modify_value_value.value2 = value2;
	modify_value_value.value3 = value3;

	ret = modify_value_clnt(modify_value_value, &result, clnt);

	if (ret != RPC_SUCCESS) {
		clnt_perror(clnt, "call failed");
		return result;
	}

	// return the error code
	return result;
}

int delete_key(int key)
{
	char *ip_tuplas = get_ip_tuplas();
	if (strcmp(ip_tuplas, "localhost") == 0)
		strcpy(ip_tuplas, localhost);

	CLIENT *clnt = clnt_create(ip_tuplas, TUPLE_SERVICE, TUPLE_SERVICE_V1, "tcp");
	if (clnt == NULL)
	{
		clnt_pcreateerror(ip_tuplas);
		exit(1);
	}

	int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	ret = delete_key_clnt(key, &result, clnt);

	if (ret != RPC_SUCCESS) {
		clnt_perror(clnt, "call failed");
		return result;
	}
	
	clnt_destroy(clnt);

	// return the error code
	return result;
}

int exist(int key)
{
	char *ip_tuplas = get_ip_tuplas();
	if (strcmp(ip_tuplas, "localhost") == 0)
		strcpy(ip_tuplas, localhost);

	CLIENT *clnt = clnt_create(ip_tuplas, TUPLE_SERVICE, TUPLE_SERVICE_V1, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror(ip_tuplas);
		exit(1);
	}

	int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	ret = exist_clnt(key, &result, clnt);

	if (ret != RPC_SUCCESS) {
		clnt_perror(clnt, "call failed");
		return result;
	}

	clnt_destroy(clnt);


	// return the error code
	return result;
}

int copy_key(int key1, int key2)
{
	char *ip_tuplas = get_ip_tuplas();
	if (strcmp(ip_tuplas, "localhost") == 0)
		strcpy(ip_tuplas, localhost);
	
	CLIENT *clnt = clnt_create(ip_tuplas, TUPLE_SERVICE, TUPLE_SERVICE_V1, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror(ip_tuplas);
		exit(1);
	}

	int result = -1;
	enum clnt_stat ret = RPC_FAILED;

	TwoKeys copy_key_keys;
	copy_key_keys.key1 = key1;
	copy_key_keys.key2 = key2;

	ret = copy_key_clnt(copy_key_keys, &result, clnt);

	if (ret != RPC_SUCCESS) {
		clnt_perror(clnt, "call failed");
		return result;
	}

	clnt_destroy(clnt);

	// return the error code
	return result;
}
