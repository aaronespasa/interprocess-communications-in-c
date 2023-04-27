/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "tuplas.h"


void
tuple_service_1(char *host)
{
	CLIENT *clnt;
	enum clnt_stat retval_1;
	int result_1;
	
	enum clnt_stat retval_2;
	int result_2;
	Value set_value_value;
	set_value_value.key = 1;
	set_value_value.value1 = "hola";
	set_value_value.value2 = 3;
	set_value_value.value3 = 4.5;
	
	// enum clnt_stat retval_3;
	// Value result_3;
	// int get_value_key;
	
	// enum clnt_stat retval_4;
	// int result_4;
	// Value modify_value_value;
	
	// enum clnt_stat retval_5;
	// int result_5;
	// int delete_key_key;
	
	// enum clnt_stat retval_6;
	// int result_6;
	// int exist_key;
	
	// enum clnt_stat retval_7;
	// int result_7;
	// TwoKeys copy_key_keys;

#ifndef	DEBUG
		clnt = clnt_create(host, TUPLE_SERVICE, TUPLE_SERVICE_V1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	retval_1 = init(&result_1, clnt);
	if (retval_1 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	} else {
		printf("init: %d\n", result_1);
	}
	
	retval_2 = set_value(set_value_value, &result_2, clnt);
	if (retval_2 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	} else {
		printf("set_value: %d\n", result_2);
	}
	
	retval_3 = get_value(get_value_key, &result_3, clnt);
	if (retval_3 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	} else {
		printf("get_value: %d\n", result_3.key);
		printf("get_value: %s\n", result_3.value1);
		printf("get_value: %d\n", result_3.value2);
		printf("get_value: %f\n", result_3.value3);
	}
	
	retval_4 = modify_value(modify_value_value, &result_4, clnt);
	if (retval_4 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	} else {
		printf("modify_value: %d\n", result_4);
	}

	retval_5 = delete_key(delete_key_key, &result_5, clnt);
	if (retval_5 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	} else {
		printf("delete_key: %d\n", result_5);
	}

	retval_6 = exist(exist_key, &result_6, clnt);
	if (retval_6 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	} else {
		printf("exist: %d\n", result_6);
	}
	
	retval_7 = copy_key(copy_key_1_keys, &result_7, clnt);
	if (retval_7 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	} else {
		printf("copy_key: %d\n", result_7);
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	tuple_service_1 (host);
exit (0);
}